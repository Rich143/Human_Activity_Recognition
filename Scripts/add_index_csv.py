import pandas as pd
import argparse

def modify_csv_index(input_file, output_file, add_index=True):
    """
    Adds or removes an index column from a CSV file.

    Parameters:
        input_file (str): Path to the input CSV.
        output_file (str): Path to save the modified CSV.
        add_index (bool): If True, add an index column. If False, remove it.
    """
    if add_index:
        # Load normally and save with index
        df = pd.read_csv(input_file)
        df.to_csv(output_file, index=True)
        print(f"Added index column and saved to {output_file}")
    else:
        # Load with index and drop it when saving
        df = pd.read_csv(input_file, index_col=0)
        df.to_csv(output_file, index=False)
        print(f"Removed index column and saved to {output_file}")

def main():
    parser = argparse.ArgumentParser(
        description="Add or remove an index column from a CSV file."
    )
    parser.add_argument("input_file", help="Path to the input CSV file")
    parser.add_argument("output_file", help="Path to save the modified CSV file")

    mode_group = parser.add_mutually_exclusive_group(required=True)
    mode_group.add_argument("--add", action="store_true", help="Add an index column")
    mode_group.add_argument("--remove", action="store_true", help="Remove an index column")

    args = parser.parse_args()

    modify_csv_index(args.input_file, args.output_file, add_index=args.add)


if __name__ == "__main__":
    main()
