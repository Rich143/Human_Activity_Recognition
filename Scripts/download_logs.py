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
from dataclasses import dataclass, asdict
from typing import Optional

# ---------- Struct and constants ----------
ROW_MARKER = 0xBEEDFACE
ROW_MARKER_LE = struct.pack('<I', ROW_MARKER)

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

# ---------- Stream parsing ----------
def read_exact(ser: serial.Serial, n: int) -> bytes:
    out = bytearray()
    while len(out) < n:
        chunk = ser.read(n - len(out))
        if not chunk:
            raise EOFError("Serial timeout/closed while reading")
        out.extend(chunk)
    return bytes(out)

def parse_rows(port: str, baud: int = 921600, csv_path: Optional[str] = None, max_rows: Optional[int] = None):
    ser = serial.Serial(port, baudrate=baud, timeout=1)
    buf = bytearray()

    writer = None
    csv_file = None
    try:
        if csv_path:
            csv_file = open(csv_path, 'w', newline='')
            writer = csv.DictWriter(csv_file, fieldnames=FIELD_NAMES[1:] + ['row_start_ok'])
            writer.writeheader()

        rows = 0
        while True:
            # Read some bytes
            buf.extend(ser.read(4096))
            if not buf:
                continue

            # Try to find the next marker
            start = buf.find(ROW_MARKER_LE)
            if start == -1:
                # keep a few trailing bytes in case marker straddles
                buf[:] = buf[-(len(ROW_MARKER_LE)-1):]
                continue

            # Discard leading garbage before the marker
            if start > 0:
                del buf[:start]

            # Need a full row
            if len(buf) < STRUCT_SIZE:
                continue

            row_bytes = bytes(buf[:STRUCT_SIZE])
            del buf[:STRUCT_SIZE]

            # Decode
            row = FlashLogRow.from_bytes(row_bytes)
            row_start_ok = (row.row_start_marker == ROW_MARKER)

            if not row_start_ok:
                # Rare: false positive marker due to random bytes. Resync by skipping one byte.
                # Put back all but the first byte and continue.
                buf[:0] = row_bytes[1:]  # push bytes (except first) back to the front
                continue

            # Use the row
            if writer:
                d = asdict(row)
                # Drop the marker in CSV (optional) and add a check flag
                d.pop('row_start_marker', None)
                d['row_start_ok'] = True
                writer.writerow(d)
            else:
                print(row)

            rows += 1
            if max_rows is not None and rows >= max_rows:
                break
    finally:
        if csv_file:
            csv_file.close()
        ser.close()

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
