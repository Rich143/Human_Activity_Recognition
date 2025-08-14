#!/usr/bin/env python3
import pandas as pd
import numpy as np
import argparse
import sys

def main():
    ap = argparse.ArgumentParser(description="Compare first X rows of two CSVs with tolerances (numeric columns only).")
    ap.add_argument("file1")
    ap.add_argument("file2")
    ap.add_argument("-n", "--rows", type=int, default=None, help="Number of rows to compare (default: all)")
    ap.add_argument("--atol", type=float, default=1e-6, help="Absolute tolerance (default: 1e-6)")
    ap.add_argument("--rtol", type=float, default=1e-9, help="Relative tolerance (default: 1e-9)")
    ap.add_argument("--report-csv", type=str, default=None, help="Optional path to write mismatch report CSV")
    args = ap.parse_args()

    # Read first N rows (assumes headers and they match)
    df1 = pd.read_csv(args.file1, nrows=args.rows)
    df2 = pd.read_csv(args.file2, nrows=args.rows)

    # Align by common columns (assume headers are identical, but guard anyway)
    cols = [c for c in df1.columns if c in df2.columns]
    df1 = df1[cols]
    df2 = df2[cols]

    # Drop boolean columns first
    bool_cols = df1.select_dtypes(include=["bool"]).columns
    df1 = df1.drop(columns=bool_cols)
    df2 = df2.drop(columns=bool_cols)

    # Convert to numeric and drop columns that fail conversion (ignore non-numeric)
    df1n = df1.apply(pd.to_numeric, errors="coerce")
    df2n = df2.apply(pd.to_numeric, errors="coerce")
    keep_cols_mask = ~(df1n.isna().any() | df2n.isna().any())
    keep_cols = list(df1n.columns[keep_cols_mask])

    if not keep_cols:
        print("[ERROR] No common numeric columns to compare after conversion.", file=sys.stderr)
        return 2
    else:
        print(f"Comparing {len(keep_cols)} numeric columns: {', '.join(keep_cols)}")

    A = df1n[keep_cols].to_numpy()
    B = df2n[keep_cols].to_numpy()

    # If row counts differ (and no -n given), compare the min
    min_rows = min(A.shape[0], B.shape[0])
    A = A[:min_rows, :]
    B = B[:min_rows, :]

    close = np.isclose(A, B, atol=args.atol, rtol=args.rtol)
    if close.all():
        print("✅ All compared values are within tolerance.")
        return 0

    # Build compact mismatch report
    where = np.argwhere(~close)
    rows = []
    for i, j in where[:1000]:  # cap console output
        rows.append({
            "row": int(i),
            "column": keep_cols[int(j)],
            "file1": float(A[i, j]),
            "file2": float(B[i, j]),
            "abs_diff": float(abs(A[i, j] - B[i, j]))
        })
    rep = pd.DataFrame(rows, columns=["row", "column", "file1", "file2", "abs_diff"])
    with pd.option_context('display.max_rows', None, 'display.max_colwidth', None):
        print("❌ Mismatches (first 1000):")
        print(rep)

    if args.report_csv:
        rep.to_csv(args.report_csv, index=False)
        print(f"[INFO] Full mismatch report written to: {args.report_csv}")

    return 1

if __name__ == "__main__":
    raise SystemExit(main())
