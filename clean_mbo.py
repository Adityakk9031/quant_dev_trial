import pandas as pd

INPUT_FILE = "mbo.csv"
OUTPUT_FILE = "cleaned_mbo.csv"

# Define column names (15 columns as per your file)
columns = [
    "ts_recv", "ts_event", "rtype", "publisher_id", "instrument_id", "action",
    "side", "price", "size", "channel_id", "order_id", "flags",
    "ts_in_delta", "sequence", "symbol"
]

def clean_csv(input_file, output_file):
    df = pd.read_csv(input_file, names=columns)

    # Fill missing price with 0 (safe default)
    df["price"] = pd.to_numeric(df["price"], errors="coerce").fillna(0).astype(int)

    # Fill missing side with 'X' (if side is empty), or drop if you prefer
    df["side"] = df["side"].fillna("X")

    # Convert size to integer safely
    df["size"] = pd.to_numeric(df["size"], errors="coerce").fillna(0).astype(int)

    # Optional: remove rows where action is not R/A/M/F/C
    df = df[df["action"].isin(["R", "A", "M", "F", "C"])]

    # Write cleaned CSV
    df.to_csv(output_file, index=False, header=False)

if __name__ == "__main__":
    clean_csv(INPUT_FILE, OUTPUT_FILE)
