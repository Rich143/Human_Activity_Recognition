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
import re
from abc import ABC, abstractmethod

@dataclass
class BaseLogRow(ABC):
    STRUCT_FMT: str
    FIELD_NAMES: list[str]
    ROW_SIZE: int

    @abstractmethod
    def row_summary(self) -> str:
        pass

    @classmethod
    def from_bytes(cls, row: bytes):
        struct_size = struct.calcsize(cls.STRUCT_FMT)  # should be 64
        assert struct_size == cls.ROW_SIZE

        vals = struct.unpack(cls.STRUCT_FMT, row)
        return cls(**dict(zip(cls.FIELD_NAMES, vals)))

    def as_dict(self) -> dict:
        """Convert this log row into a dictionary suitable for csv.DictWriter."""
        return asdict(self)

@dataclass
class ErrorLogRow(BaseLogRow):
    row_start_marker: int
    timestamp_ms: int
    error_code: int
    data: int
    padding: int

    # < means little-endian, packed layout
    STRUCT_FMT = "<IIHIH"
    ROW_SIZE = 16
    FIELD_NAMES = [
        "row_start_marker",
        "timestamp_ms",
        "error_code",
        "data",
        "padding",
    ]

    ERROR_REGISTRY = {
    0: ("IMU_READ_ERROR", "BSP Error Code"),
    1: ("IMU_INIT_ERROR", "BSP Error Code"),
    2: ("PREPROCESS_ERROR", "Preprocess Status"),
    3: ("AI_RUN_ERROR", "None"),
    4: ("POSTPROCESS_ERROR", "None"),
    5: ("FLASH_LOG_INIT_ERROR", "flash_log_status_t"),
    6: ("FLASH_LOG_WRITE_ERROR", "flash_log_status_t"),
    7: ("CLI_INIT_ERROR", "cli_status_t"),
    8: ("CLI_START_ERROR", "cli_status_t"),
    9: ("CLI_RUN_ERROR", "cli_status_t"),
    10: ("CLI_FREERTOS_ASSERT_ERROR", "Line Number"),
    11: ("BLE_INIT_ERROR", "BLE Error Code"),
    12: ("BLE_NOTIFY_ERROR", "BLE Error Code"),
    }

    @property
    def decoded_error(self):
        name, label = self.ERROR_REGISTRY.get(self.error_code, ("UNKNOWN", "UNKNOWN"))
        return {"name": name, "data_label": label, "data_value": self.data}

    def as_dict(self):
        base = super().as_dict()
        decoded = self.decoded_error
        base.update({
            "error_name": decoded["name"],
            "error_data_label": decoded["data_label"],
            "error_data_value": decoded["data_value"],
        })
        return base

    def row_summary(self):
        return f"Error: {self.decoded_error['name']} ({self.decoded_error['data_label']}={self.decoded_error['data_value']})"

@dataclass
class DataLogRow(BaseLogRow):
    row_start_marker: int
    unproc_x: float; unproc_y: float; unproc_z: float
    lowpass_filtered_x: float; lowpass_filtered_y: float; lowpass_filtered_z: float
    proc_x: float; proc_y: float; proc_z: float
    model_output_0: float; model_output_1: float; model_output_2: float; model_output_3: float
    output_class: int
    contains_output: int

    STRUCT_FMT = '<I13fII'
    ROW_SIZE = 64

    FIELD_NAMES = [
        'row_start_marker',
        'unproc_x', 'unproc_y', 'unproc_z',
        'lowpass_filtered_x', 'lowpass_filtered_y', 'lowpass_filtered_z',
        'proc_x', 'proc_y', 'proc_z',
        'model_output_0', 'model_output_1', 'model_output_2', 'model_output_3',
        'output_class',
        'contains_output',
    ]

    def row_summary(self):
        return f"Unprocessed: ({self.unproc_x:.2f}, {self.unproc_y:.2f}, {self.unproc_z:.2f})"

class BaseLog(ABC):
    log_row_type: type[BaseLogRow]

    @abstractmethod
    def log_size_cmd(self) -> str:
        pass

    # TODO: This should have some sort of end marker (so we know we've got the whole response before doing re search)
    @abstractmethod
    def log_size_re(self) -> re.Pattern:
        pass

class DataLogs(BaseLog):
    LOG_SIZE_COMMAND = 'logSize\n'
    LOG_NUM_ROWS_RE = re.compile(rb'Num\s*Rows:\s*(\d+)',
                                 re.IGNORECASE)

    log_row_type = DataLogRow

    def log_size_cmd(self) -> str:
        return self.LOG_SIZE_COMMAND

    def log_size_re(self) -> re.Pattern:
        return self.LOG_NUM_ROWS_RE

class ErrorLogs(BaseLog):
    LOG_SIZE_COMMAND = 'errorLogSize\n'
    LOG_NUM_ROWS_RE = re.compile(rb'Num\s*Rows:\s*(\d+)',
                                 re.IGNORECASE)

    log_row_type = ErrorLogRow

    def log_size_cmd(self) -> str:
        return self.LOG_SIZE_COMMAND

    def log_size_re(self) -> re.Pattern:
        return self.LOG_NUM_ROWS_RE

class CLI:
    # ---------- Struct and constants ----------
    CLI_PROMPT =  'CLI > '
    CLI_ENABLE_CODE = 'aaaaaaaaaa'

    CLI_PING_COMMAND = 'ping\n'
    # CLI_LOG_SIZE_COMMAND = 'logSize\n'

    # LOG_NUM_ROWS_RE = re.compile(rb'Num\s*Rows:\s*(\d+)', re.IGNORECASE)

    ROW_MARKER = 0xBEEDFACE
    ROW_MARKER_LE = struct.pack('<I', ROW_MARKER)

    LOG_RECEIVE_TIMEOUT_SECONDS = 2

    def __init__(self, ser: serial.Serial, log: BaseLog,
                 csv_path: Optional[str] = None,
                 max_rows: Optional[int] = None):
        self.ser = ser
        self.log = log
        self.csv_path = csv_path
        self.max_rows = max_rows

    def open_csv(self):
        if self.csv_path is None:
            return None, None

        csv_file = open(self.csv_path, 'w',
                        newline='')

        low_row_type = self.log.log_row_type

        writer = csv.DictWriter(csv_file, fieldnames=low_row_type.FIELD_NAMES[1:])
        writer.writeheader()
        print(f"[*] CSV logging to: {self.csv_path}")

        return writer, csv_file

    # ---------- Helpers ----------
    def print_ascii_chunk(self, chunk: bytes):
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

    def send_cli_command(self, command: str):
        self.ser.write(command.encode('ascii'))
        self.ser.flush()
        time.sleep(0.1)

    def enable_cli_check_for_enabled(self) -> bool:
        self.send_cli_command(self.CLI_ENABLE_CODE)
        time.sleep(1)

        self.ser.reset_input_buffer()

        self.send_cli_command(self.CLI_PING_COMMAND)

        start = time.time()

        rx = ""
        while time.time() - start < 10:
            rx += self.ser.read(5000).decode('ascii', errors='ignore')
            if "pong" in rx and self.CLI_PROMPT in rx:
                self.ser.reset_input_buffer()
                return True

        # Failed to enable CLI
        self.ser.reset_input_buffer()
        return False

    def query_log_size(self, timeout: float=2.0) -> int:
        """
        Sends 'logSize' command, then parses 'Num Rows: N'.
        Returns the integer row count. Raises TimeoutError if not found in time.
        """

        self.send_cli_command(self.log.log_size_cmd())

        buf = bytearray()
        deadline = time.time() + timeout
        while time.time() < deadline:
            chunk = self.ser.read(64)
            if chunk:
                buf.extend(chunk)
                m = self.log.log_size_re().search(buf)
                if m:
                    rows = int(m.group(1))
                    self.ser.reset_input_buffer()
                    return rows
        # If we got here, we didn’t see the line in time—print whatever we saw for debugging.
        try:
            print("[!] logSize response (partial):", buf.decode('ascii', errors='ignore'))
        except Exception:
            pass
        raise TimeoutError("Timed out waiting for 'Num Rows:' from device")

    def read_chunk_into_buf(self, buf):
        chunk = self.ser.read(64)
        if chunk:
            buf.extend(chunk)

    def handle_start_not_found(self, buf):
        # keep only last 3 bytes so a split marker can straddle reads
        if len(buf) > 3:
            del buf[:-3]

        return buf

    def trim_buf_to_start(self, buf, start):
        if start > 0:
            print(f"[*] Found marker at offset {start}, discarded {start} byte(s) of preamble.")
            del buf[:start]

        return buf

    def pop_row_and_decode(self, buf: bytearray):
        """Pop one log row from the buffer and decode it into a log row object."""
        row_type = self.log.log_row_type
        row_size = row_type.ROW_SIZE

        if len(buf) < row_size:
            raise ValueError(f"Not enough bytes in buffer for {row_type.__name__}")

        row_bytes = bytes(buf[:row_size])
        del buf[:row_size]

        row = row_type.from_bytes(row_bytes)
        return row, buf

    # ---------- Stream parsing ----------
    def download_logs(self):
        log_row_type = self.log.log_row_type

        # --- Enable CLI ---
        print("[*] Enabling CLI (sending wake combo)...")
        enabled = self.enable_cli_check_for_enabled()
        if not enabled:
            print("[!!] CLI is not enabled. Aborting.")
            return
        else:
            print("[*] CLI is enabled.")

        # --- Query log size ---
        print("[*] Querying log size...")
        num_rows = self.query_log_size()
        print(f"[*] Device reports {num_rows} rows.")

        # # --- Kick off dump ---
        cmd = 'logDump\n'
        self.send_cli_command(cmd)

        print("[*] Waiting for first row marker 0x%08X ..." % self.ROW_MARKER)

        buf = bytearray()
        writer, csv_file = self.open_csv()
        synced = False  # becomes True after we lock onto the first marker
        rows = 0
        last_status_print = 0.0
        last_row_received_time = time.time()

        try:
            while True:
                self.read_chunk_into_buf(buf)

                # Status ping while waiting
                now = time.time()
                if not synced and (now - last_status_print) > 0.5:
                    print(f"[.] still waiting for marker… buffer has {len(buf)} bytes")
                    last_status_print = now

                if now - last_row_received_time > self.LOG_RECEIVE_TIMEOUT_SECONDS:
                    print("[!] Timeout. Stopping.")
                    break


                # Try to find the next marker
                start = buf.find(self.ROW_MARKER_LE)
                if start == -1:
                    buf = self.handle_start_not_found(buf)
                    continue

                buf = self.trim_buf_to_start(buf, start)

                # Need a full row
                if len(buf) < log_row_type.ROW_SIZE:
                    continue

                # We have at least one row; from this point on consider ourselves synced.
                if not synced:
                    print("[*] Synced on first row marker.")
                    synced = True

                row, buf = self.pop_row_and_decode(buf)

                # Use the row
                if writer:
                    # TODO: Add timestamp
                    d = row.as_dict()
                    d.pop('row_start_marker', None)
                    writer.writerow(d)

                rows += 1

                last_row_received_time = time.time()

                if rows <= 10 or (rows % 100) == 0:
                    msg = f"[#] Row {rows} / {num_rows}: "
                    msg += row.row_summary()
                    print(msg)

                if self.max_rows is not None and rows >= self.max_rows:
                    print("[*] Reached max_rows, stopping.")
                    break

        finally:
            if csv_file:
                csv_file.close()
                print("[*] CSV closed.")
            self.ser.close()
            print("[*] Serial port closed. Total rows:", rows)

def open_serial_port(port: str, baud: int = 921600):
    ser = serial.Serial(port, baudrate=baud, timeout=0.01)
    return ser

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

    ser = open_serial_port(port, baud)
    log = DataLogs()

    cli = CLI(ser, log, csv_out)
    cli.download_logs()
