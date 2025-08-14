#!/usr/bin/env python3
"""
Decode flash_log_row_t structs from a UART stream.

C struct (total 64 bytes, little-endian):
typedef struct {
    uint32_t row_start_marker;         // 0xBEEDFACE
    float unproc_x, unproc_y, unproc_z;
    float lowpass_filtered_x, lowpass_filtered_y, lowpass_filtered_z;
    float proc_x, proc_y, proc_z;
    float model_output[4];
    uint32_t output_class;
    uint32_t contains_output;
} flash_log_row_t;
"""
import serial
import struct
import sys
import csv
import time
from dataclasses import dataclass, asdict
from typing import Optional

# ---------- Struct and constants ----------
CLI_PROMPT =  'CLI > '
CLI_ENABLE_CODE = 'aaaaaaaaaa'

CLI_PING_COMMAND = 'ping\n'

ROW_MARKER = 0xBEEDFACE
ROW_MARKER_LE = struct.pack('<I', ROW_MARKER)

LOG_RECEIVE_TIMEOUT_SECONDS = 5

# < = little-endian. Layout: I (marker) + 13f + I + I  => 64 bytes
STRUCT_FMT = '<I13fII'
STRUCT_SIZE = struct.calcsize(STRUCT_FMT)  # should be 64
assert STRUCT_SIZE == 64, STRUCT_SIZE

FIELD_NAMES = [
    'row_start_marker',
    'unproc_x', 'unproc_y', 'unproc_z',
    'lowpass_filtered_x', 'lowpass_filtered_y', 'lowpass_filtered_z',
    'proc_x', 'proc_y', 'proc_z',
    'model_output_0', 'model_output_1', 'model_output_2', 'model_output_3',
    'output_class',
    'contains_output',
]

@dataclass
class FlashLogRow:
    row_start_marker: int
    unproc_x: float; unproc_y: float; unproc_z: float
    lowpass_filtered_x: float; lowpass_filtered_y: float; lowpass_filtered_z: float
    proc_x: float; proc_y: float; proc_z: float
    model_output_0: float; model_output_1: float; model_output_2: float; model_output_3: float
    output_class: int
    contains_output: int

    @classmethod
    def from_bytes(cls, row: bytes) -> "FlashLogRow":
        vals = struct.unpack(STRUCT_FMT, row)
        return cls(**dict(zip(FIELD_NAMES, vals)))

# ---------- Helpers ----------
def print_ascii_chunk(chunk: bytes):
    """Print human-readable ASCII seen before binary starts (echo, prompts, etc.)."""
    if not chunk:
        return
    # Keep CR/LF; drop non-ASCII
    text = chunk.decode('ascii', errors='ignore')
    if not text.strip():
        return
    # Print line by line with prefix
    for line in text.splitlines(True):
        if line.strip() == "":
            continue
        sys.stdout.write("[ASCII] " + line)
        sys.stdout.flush()

def send_cli_command(command: str, ser: serial.Serial):
    ser.write(command.encode('ascii'))
    ser.flush()
    time.sleep(0.1)

def enable_cli_check_for_enabled(ser: serial.Serial) -> bool:
    send_cli_command(CLI_ENABLE_CODE, ser)
    time.sleep(1)

    ser.reset_input_buffer()

    send_cli_command(CLI_PING_COMMAND, ser)

    start = time.time()

    rx = ""
    while time.time() - start < 10:
        rx += ser.read(5000).decode('ascii', errors='ignore')
        if "pong" in rx and CLI_PROMPT in rx:
            ser.reset_input_buffer()
            return True

    # Failed to enable CLI
    ser.reset_input_buffer()
    return False

def read_chunk_into_buf(ser, buf):
    chunk = ser.read(4096)
    if chunk:
        buf.extend(chunk)

def handle_start_not_found(buf):
    # keep only last 3 bytes so a split marker can straddle reads
    if len(buf) > 3:
        del buf[:-3]

    return buf

def trim_buf_to_start(buf, start):
    if start > 0:
        print(f"[*] Found marker at offset {start}, discarded {start} byte(s) of preamble.")
        del buf[:start]

    return buf

def pop_row_and_decode(buf):
    row_bytes = bytes(buf[:STRUCT_SIZE])
    del buf[:STRUCT_SIZE]

    # Decode
    row = FlashLogRow.from_bytes(row_bytes)
    row_start_ok = (row.row_start_marker == ROW_MARKER)

    return row, buf

    # if row_start_ok:
        # return row, buf
    # else:
        # # False positive; resync by skipping one byte.
        # buf[:0] = row_bytes[1:]  # push bytes (except first) back
        # print("[!] Marker check failed after unpack; resyncing by 1 byte.")


# ---------- Stream parsing ----------
def parse_rows(port: str, baud: int = 921600, csv_path: Optional[str] = None, max_rows: Optional[int] = None):
    print(f"[*] Opening {port} @ {baud} ...")
    ser = serial.Serial(port, baudrate=baud, timeout=0.02)
    print("[*] Port opened.")

    # --- Enable CLI ---
    print("[*] Enabling CLI (sending wake combo)...")
    enabled = enable_cli_check_for_enabled(ser)
    if not enabled:
        print("[!!] CLI is not enabled. Aborting.")
        return
    else:
        print("[*] CLI is enabled.")

    # # --- Kick off dump ---
    cmd = 'logDump\n'
    send_cli_command(cmd, ser)

    print("[*] Waiting for first row marker 0x%08X ..." % ROW_MARKER)

    buf = bytearray()
    writer = None
    csv_file = None
    synced = False  # becomes True after we lock onto the first marker
    rows = 0
    last_status_print = 0.0
    last_row_received_time = time.time()

    try:
        if csv_path:
            csv_file = open(csv_path, 'w', newline='')
            writer = csv.DictWriter(csv_file, fieldnames=FIELD_NAMES[1:] + ['row_start_ok'])
            writer.writeheader()
            print(f"[*] CSV logging to: {csv_path}")

        while True:
            read_chunk_into_buf(ser, buf)

            # Status ping while waiting
            now = time.time()
            if not synced and (now - last_status_print) > 0.5:
                print(f"[.] still waiting for marker… buffer has {len(buf)} bytes")
                last_status_print = now

            if now - last_row_received_time > LOG_RECEIVE_TIMEOUT_SECONDS:
                print("[!] Timeout. Stopping.")
                break


            # Try to find the next marker
            start = buf.find(ROW_MARKER_LE)
            if start == -1:
                buf = handle_start_not_found(buf)
                continue

            buf = trim_buf_to_start(buf, start)

            # Need a full row
            if len(buf) < STRUCT_SIZE:
                continue

            # We have at least one row; from this point on consider ourselves synced.
            if not synced:
                print("[*] Synced on first row marker.")
                synced = True

            row, buf = pop_row_and_decode(buf)

            # Use the row
            if writer:
                d = asdict(row)
                d.pop('row_start_marker', None)
                d['row_start_ok'] = True
                writer.writerow(d)

            rows += 1

            print(f"[#] Row {rows}  class={row.output_class}  has_out={row.contains_output}  "
                  f"unproc=({row.unproc_x:.6f},{row.unproc_y:.6f},{row.unproc_z:.6f})")

            if max_rows is not None and rows >= max_rows:
                print("[*] Reached max_rows, stopping.")
                break

    finally:
        if csv_file:
            csv_file.close()
            print("[*] CSV closed.")
        ser.close()
        print("[*] Serial port closed. Total rows:", rows)

# ---------- CLI ----------
if __name__ == "__main__":
    # Examples:
    #   python decode_flash_rows.py /dev/tty.usbmodemXXXX 921600 out.csv
    #   python decode_flash_rows.py /dev/tty.usbserial-0001 2000000
    if len(sys.argv) < 2:
        print("Usage: python decode_flash_rows.py <serial_port> [baud=921600] [csv_out]")
        sys.exit(1)
    port = sys.argv[1]
    baud = int(sys.argv[2]) if len(sys.argv) >= 3 else 921600
    csv_out = sys.argv[3] if len(sys.argv) >= 4 else None
    parse_rows(port, baud, csv_out)
