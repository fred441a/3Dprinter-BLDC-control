import csv
import re

filename = "scope_2.csv"
savefile = "scope_2_means.csv"

rows = []

# --- Read CSV (auto detect separator) ---
with open(filename, 'r', encoding='utf-8-sig') as csvfile:
    first_line = csvfile.readline()
    sep = ',' if ',' in first_line else ('\t' if '\t' in first_line else None)
    if sep is None:
        sep = r"\s+"

    csvfile.seek(0)
    if sep == r"\s+":
        next(csvfile)
        for line in csvfile:
            parts = re.split(sep, line.strip())
            if len(parts) >= 2:
                try:
                    t = float(parts[0])
                    v = float(parts[1])
                    rows.append((t, v))
                except ValueError:
                    continue
    else:
        csvreader = csv.reader(csvfile, delimiter=sep)
        header = next(csvreader)
        for row in csvreader:
            if len(row) >= 2:
                try:
                    t = float(row[0])
                    v = float(row[1])
                    rows.append((t, v))
                except ValueError:
                    continue

print(f"✅ Total samples read: {len(rows)}")

# --- Compute mean of binarized samples ---
group_size = 11
averaged_data = []

for i in range(0, len(rows), group_size):
    chunk = rows[i:i + group_size]
    if len(chunk) > 0:
        first_time = chunk[0][0]
        # Binarize each value in the chunk
        binarized = [0 if v < 2 else 10 for _, v in chunk]
        mean_binarized = round(sum(binarized) / len(binarized), 3)
        averaged_data.append((first_time, mean_binarized))

print(f"✅ Computed {len(averaged_data)} averaged values")

# --- Save Excel-friendly CSV ---
with open(savefile, 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(["time_s", "mean_volt"])
    for t, m in averaged_data:
        csvwriter.writerow([f"{t:.3f}", f"{m}"])

print(f"💾 Saved averaged binarized data to: {savefile}")
