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
from typing import Optional, ClassVar
import re
from abc import ABC, abstractmethod
import argparse

@dataclass
class BaseLogRow(ABC):
    STRUCT_FMT: ClassVar[str]
    FIELD_NAMES: ClassVar[list[str]]
    CSV_FIELDS: ClassVar[list[str]]
    ROW_SIZE: ClassVar[int]

    @abstractmethod
    def row_summary(self) -> str:
        pass

    @classmethod
    def from_bytes(cls, row: bytes):
        struct_size = struct.calcsize(cls.STRUCT_FMT)  # should be 64
        assert struct_size == cls.ROW_SIZE

        vals = struct.unpack(cls.STRUCT_FMT, row)
        return cls(**dict(zip(cls.FIELD_NAMES, vals)))

    def csv_dict(self) -> dict:
        """
        Convert this log row into a dictionary suitable for csv.DictWriter.
        This should be a supersert of CSV_FIELDS
        """
        return asdict(self)

@dataclass
class ErrorLogRow(BaseLogRow):
    row_start_marker: int
    timestamp_ms: int
    error_code: int
    data: int
    padding: int

    # < means little-endian, packed layout
    STRUCT_FMT: ClassVar[str] = "<IIHIH"
    ROW_SIZE: ClassVar[int] = 16
    FIELD_NAMES: ClassVar[list[str]] = [
        "row_start_marker",
        "timestamp_ms",
        "error_code",
        "data",
        "padding",
    ]

    CSV_FIELDS: ClassVar[list[str]] = [
        "timestamp_ms",
        "error_code",
        "error_name",
        "error_data_label",
        "error_data_value",
    ]

    ERROR_REGISTRY = {
        0: {"name": "IMU_READ_ERROR", "label": "BSP Error Code", "type": "int"},
        1: {"name": "IMU_INIT_ERROR", "label": "BSP Error Code", "type": "int"},
        2: {"name": "AI_NOT_INITIALIZED", "label": "None", "type": "uint"},
        3: {"name": "PREPROCESS_ERROR", "label": "Preprocess Status", "type": "uint"},
        4: {"name": "AI_RUN_ERROR", "label": "None", "type": "uint"},
        5: {"name": "POSTPROCESS_ERROR", "label": "None", "type": "uint"},
        6: {"name": "FLASH_LOG_INIT_ERROR", "label": "flash_log_status_t", "type": "uint"},
        7: {"name": "FLASH_LOG_WRITE_ERROR", "label": "flash_log_status_t", "type": "uint"},
        8: {"name": "CLI_INIT_ERROR", "label": "cli_status_t", "type": "uint"},
        9: {"name": "CLI_START_ERROR", "label": "cli_status_t", "type": "uint"},
        10: {"name": "CLI_RUN_ERROR", "label": "cli_status_t", "type": "uint"},
        11: {"name": "CLI_FREERTOS_ASSERT_ERROR", "label": "Line Number", "type": "uint"},
        12: {"name": "BLE_INIT_ERROR", "label": "BLE Error Code", "type": "int"},
        13: {"name": "BLE_NOTIFY_ERROR", "label": "BLE Error Code", "type": "int"},
    }

    @property
    def decoded_error(self):
        info = self.ERROR_REGISTRY.get(
            self.error_code,
            {"name": "UNKNOWN", "label": "UNKNOWN", "type": "uint"},
        )
        data_value = self.data

        # Interpret signed vs unsigned using struct
        if info["type"] == "int":
            data_value = struct.unpack("<i", struct.pack("<I", self.data))[0]  # signed 32-bit
        else:
            data_value = struct.unpack("<I", struct.pack("<I", self.data))[0]  # unsigned 32-bit

        return {
            "error_name": info["name"],
            "error_data_label": info["label"],
            "error_data_value": data_value,
            "error_data_type": info["type"],
        }

    def csv_dict(self):
        base = super().csv_dict()
        decoded = self.decoded_error
        base.update(decoded)
        return base

    def row_summary(self):
        return f"Error: {self.decoded_error['error_name']} ({self.decoded_error['error_data_label']}={self.decoded_error['error_data_value']})"

@dataclass
class DataLogRow(BaseLogRow):
    row_start_marker: int
    unproc_x: float; unproc_y: float; unproc_z: float
    lowpass_filtered_x: float; lowpass_filtered_y: float; lowpass_filtered_z: float
    proc_x: float; proc_y: float; proc_z: float
    model_output_0: float; model_output_1: float; model_output_2: float; model_output_3: float
    output_class: int
    contains_output: int

    STRUCT_FMT: ClassVar[str] = '<I13fII'
    ROW_SIZE: ClassVar[int] = 64
    FIELD_NAMES: ClassVar[list[str]] = [
        'row_start_marker',
        'unproc_x', 'unproc_y', 'unproc_z',
        'lowpass_filtered_x', 'lowpass_filtered_y', 'lowpass_filtered_z',
        'proc_x', 'proc_y', 'proc_z',
        'model_output_0', 'model_output_1', 'model_output_2', 'model_output_3',
        'output_class',
        'contains_output',
    ]

    CSV_FIELDS: ClassVar[list[str]] = [
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
    LOG_SIZE_COMMAND: str
    LOG_DUMP_COMMAND: str

    LOG_NUM_ROWS_RE = re.compile(r'Num\s*Rows:\s*(\d+) rows.',
                                 re.IGNORECASE)

    def log_size_re(self) -> re.Pattern:
        return self.LOG_NUM_ROWS_RE

    def log_size_cmd(self) -> str:
        return self.LOG_SIZE_COMMAND

    def log_dump_cmd(self) -> str:
        return self.LOG_DUMP_COMMAND

class DataLogs(BaseLog):
    LOG_SIZE_COMMAND = 'logSize\n'
    LOG_DUMP_COMMAND = 'logDump\n'

    log_row_type = DataLogRow

class ErrorLogs(BaseLog):
    LOG_SIZE_COMMAND = 'errorLogSize\n'
    LOG_DUMP_COMMAND = 'dumpErrorLogs\n'


    log_row_type = ErrorLogRow


class LogDownloader:
    # ---------- Struct and constants ----------
    CLI_PROMPT =  'CLI > '
    CLI_ENABLE_CODE = 'aaaaaaaaaa'

    CLI_PING_COMMAND = 'ping\n'

    ROW_MARKER = 0xBEEDFACE
    ROW_MARKER_LE = struct.pack('<I', ROW_MARKER)

    LOG_RECEIVE_TIMEOUT_SECONDS = 2

    def __init__(self, ser: serial.Serial, log: BaseLog,
                 csv_path: Optional[str] = None,
                 max_rows: Optional[int] = None,
                 verbose: bool = False):
        self.ser = ser
        self.log = log
        self.csv_path = csv_path
        self.max_rows = max_rows
        self.verbose = verbose

        self.last_sync_status_print = 0
        self.last_row_received_time = time.time()

    def open_csv(self):
        if self.csv_path is None:
            return None, None

        csv_file = open(self.csv_path, 'w',
                        newline='')

        log_row_type = self.log.log_row_type

        writer = csv.DictWriter(csv_file, fieldnames=log_row_type.CSV_FIELDS,
                                extrasaction='ignore')
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
        self.verbose_print("[*] Sending CLI command: ", command)
        self.ser.write(command.encode('ascii'))
        self.ser.flush()
        time.sleep(0.1)

    def verbose_print(self, *args, **kwargs):
        if self.verbose:
            print(*args, **kwargs)

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
                self.verbose_print("[*] CLI rx:", rx)

                self.ser.reset_input_buffer()
                return True

        # Failed to enable CLI
        self.ser.reset_input_buffer()

        self.verbose_print("[*] CLI failed to enable. CLI rx:", rx)

        return False

    def query_log_size(self, timeout: float=2.0) -> int:
        """
        Sends 'logSize' command, then parses 'Num Rows: N'.
        Returns the integer row count. Raises TimeoutError if not found in time.
        """

        self.send_cli_command(self.log.log_size_cmd())

        rx = ""
        deadline = time.time() + timeout
        while time.time() < deadline:
            rx += self.ser.read(64).decode('ascii', errors='ignore')

            m = self.log.log_size_re().search(rx)
            if m:
                rows = int(m.group(1))
                self.ser.reset_input_buffer()

                self.verbose_print("[*] Received logSize response:", rx)
                return rows
        # If we got here, we didn’t see the line in time—print whatever we saw for debugging.
        try:
            print("[!] logSize response (partial):", rx)
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

        self.last_row_received_time = time.time()

        return row, buf

    def print_sync_status(self, buf):
        now = time.time()

        if (now - self.last_sync_status_print) > 0.5:
            print(f"[.] still waiting for marker… buffer has {len(buf)} bytes")
            self.last_sync_status_print = now

    def download_timeout(self) -> bool:
        now = time.time()
        if now - self.last_row_received_time > self.LOG_RECEIVE_TIMEOUT_SECONDS:
            print("[!] Timeout. Stopping.")
            return True

        return False


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
        cmd = self.log.log_dump_cmd()
        self.send_cli_command(cmd)

        print("[*] Waiting for first row marker 0x%08X ..." % self.ROW_MARKER)

        buf = bytearray()
        writer, csv_file = self.open_csv()
        synced = False  # becomes True after we lock onto the first marker
        rows = 0

        self.last_row_received_time = time.time()

        try:
            while True:
                self.read_chunk_into_buf(buf)

                # Status ping while waiting
                if not synced:
                    self.print_sync_status(buf)

                if self.download_timeout():
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
                    d = row.csv_dict()
                    d.pop('row_start_marker', None)
                    writer.writerow(d)

                rows += 1

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
    parser = argparse.ArgumentParser(
        description="Decode flash rows from a serial port."
    )

    # Named arguments with short and long flags
    parser.add_argument(
        "-p", "--port",
        required=True,
        help="Serial port device (e.g., /dev/tty.usbmodemXXXX)"
    )
    parser.add_argument(
        "-b", "--baud",
        type=int,
        default=921600,
        help="Baud rate (default: 921600)"
    )
    parser.add_argument(
        "-c", "--csv",
        default=None,
        help="Optional CSV output file"
    )

    parser.add_argument(
        "--max-rows",
        type=int,
        default=None,
        help="Maximum number of rows to download"
    )

    parser.add_argument(
        "-t", "--log-type",
        choices=["data", "error"],
        default="data",
        help="Which type of log to decode: 'data' or 'error'"
    )

    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Verbose output"
    )

    args = parser.parse_args()

    # Access arguments
    port = args.port
    baud = args.baud
    csv_out = args.csv
    max_rows = args.max_rows
    verbose = args.verbose

    ser = open_serial_port(port, baud)

    if args.log_type == "data":
        log = DataLogs()
    elif args.log_type == "error":
        log = ErrorLogs()
    else:
        raise ValueError(f"Unknown log type: {args.log_type}")

    downloader = LogDownloader(ser, log,
                               csv_out, max_rows, verbose)
    downloader.download_logs()
