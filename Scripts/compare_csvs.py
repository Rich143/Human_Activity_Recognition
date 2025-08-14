
#!/usr/bin/env python3
import pandas as pd
import numpy as np
import argparse
import sys
from typing import List, Tuple, Optional

def read_csv(path: str, nrows: Optional[int], delimiter: Optional[str], header: Optional[str]) -> pd.DataFrame:
    hdr = "infer" if header is None else (0 if header.lower() == "yes" else None)
    df = pd.read_csv(path, nrows=nrows, sep=delimiter, engine="python", header=hdr)
    return df

def align_columns(df1: pd.DataFrame, df2: pd.DataFrame) -> Tuple[pd.DataFrame, pd.DataFrame, List[str], bool]:
    """Return (df1, df2_aligned, columns, using_headers)"""
    if list(df1.columns) == list(df2.columns):
        return df1, df2.reindex(columns=df1.columns), list(df1.columns), True
    # Try set-equality on headers
    if set(df1.columns) == set(df2.columns) and len(df1.columns) == len(df2.columns):
        cols = list(df1.columns)
        return df1, df2.reindex(columns=cols), cols, True
    # Fall back to positional alignment
    min_cols = min(df1.shape[1], df2.shape[1])
    cols = [f"col_{i}" for i in range(min_cols)]
    df1_pos = df1.iloc[:, :min_cols].copy()
    df2_pos = df2.iloc[:, :min_cols].copy()
    df1_pos.columns = cols
    df2_pos.columns = cols
    return df1_pos, df2_pos, cols, False

def isclose_series(a: pd.Series, b: pd.Series, atol: float, rtol: float) -> pd.Series:
    a_num = pd.to_numeric(a, errors="coerce")
    b_num = pd.to_numeric(b, errors="coerce")
    both_num = a_num.notna() & b_num.notna()
    both_nan = a.isna() & b.isna()  # Treat NaN==NaN as equal
    
    eq = pd.Series(False, index=a.index)
    # Numeric comparisons with tolerance
    if both_num.any():
        eq.loc[both_num] = np.isclose(a_num[both_num].to_numpy(), b_num[both_num].to_numpy(), atol=atol, rtol=rtol)
    # Non-numeric: exact string match (including empty vs empty), excluding NaNs handled above
    non_num = ~(both_num | both_nan)
    if non_num.any():
        eq.loc[non_num] = (a.astype(str).fillna("") == b.astype(str).fillna("")).loc[non_num]
    # NaN equals NaN
    if both_nan.any():
        eq.loc[both_nan] = True
    return eq

def compare_dataframes(df1: pd.DataFrame, df2: pd.DataFrame, atol: float, rtol: float, show_examples: int) -> Tuple[bool, pd.DataFrame]:
    df1_aligned, df2_aligned, columns, used_headers = align_columns(df1, df2)
    mismatches = []
    for col in columns:
        eq_mask = isclose_series(df1_aligned[col], df2_aligned[col], atol=atol, rtol=rtol)
        if not eq_mask.all():
            mismatch_idx = np.where(~eq_mask.to_numpy())[0]
            for i in mismatch_idx[:show_examples]:
                mismatches.append({
                    "row_index": int(i),
                    "column": col,
                    "file1_value": df1_aligned.iloc[i][col],
                    "file2_value": df2_aligned.iloc[i][col]
                })
    report = pd.DataFrame(mismatches, columns=["row_index", "column", "file1_value", "file2_value"])
    ok = report.empty
    return ok, report

def main(argv: Optional[List[str]] = None) -> int:
    p = argparse.ArgumentParser(description="Compare the first X rows of two CSV files with tolerances.")
    p.add_argument("file1", help="Path to first CSV")
    p.add_argument("file2", help="Path to second CSV")
    p.add_argument("-n", "--rows", type=int, default=None, help="Number of rows to compare (default: all)")
    p.add_argument("--atol", type=float, default=1e-6, help="Absolute tolerance (default: 1e-6)")
    p.add_argument("--rtol", type=float, default=1e-9, help="Relative tolerance (default: 1e-9)")
    p.add_argument("--delimiter", type=str, default=None, help="CSV delimiter (default: auto)")
    p.add_argument("--header", type=str, choices=["yes", "no"], default=None, help="Whether CSVs have a header row (default: auto-detect)")
    p.add_argument("--show-examples", type=int, default=20, help="Max mismatches to print (default: 20)")
    p.add_argument("--report-csv", type=str, default=None, help="Optional path to write full mismatch report CSV")
    args = p.parse_args(argv)

    try:
        df1 = read_csv(args.file1, nrows=args.rows, delimiter=args.delimiter, header=args.header)
        df2 = read_csv(args.file2, nrows=args.rows, delimiter=args.delimiter, header=args.header)
    except Exception as e:
        print(f"[ERROR] Failed to read CSVs: {e}", file=sys.stderr)
        return 2

    # If row counts differ after truncation, warn (we still compare on min length)
    min_len = min(len(df1), len(df2))
    if args.rows is not None:
        # ensure both are same length up to n rows
        df1 = df1.iloc[:min_len].copy()
        df2 = df2.iloc[:min_len].copy()
    else:
        if len(df1) != len(df2):
            min_len = min(len(df1), len(df2))
            print(f"[WARN] Different row counts: file1={len(df1)}, file2={len(df2)}. Comparing first {min_len} rows.")
            df1 = df1.iloc[:min_len].copy()
            df2 = df2.iloc[:min_len].copy()

    ok, report = compare_dataframes(df1, df2, atol=args.atol, rtol=args.rtol, show_examples=args.show_examples)

    if ok:
        print("✅ All compared values are within tolerance.")
        return 0
    else:
        print("❌ Mismatches found (showing up to --show-examples):")
        with pd.option_context('display.max_rows', None, 'display.max_colwidth', None):
            print(report)
        if args.report_csv:
            try:
                report.to_csv(args.report_csv, index=False)
                print(f"[INFO] Full mismatch report written to: {args.report_csv}")
            except Exception as e:
                print(f"[WARN] Could not write report CSV: {e}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
