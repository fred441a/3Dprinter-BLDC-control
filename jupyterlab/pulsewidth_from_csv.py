import csv
filename = "scope_2.csv";
fields = []  # Column names
rows = []    # Data rows

with open(filename, 'r') as csvfile:
    csvreader = csv.reader(csvfile)  # Reader object

    fields = next(csvreader)  # Read header
    for row in csvreader:     # Read rows
        rows.append(row)

    print("Total no. of rows: %d" % csvreader.line_num)  # Row count

print('Field names are: ' + ', '.join(fields))

print('\nFirst 5 rows are:\n')
for row in rows[:5]:
    for col in row:
        print("%10s" % col, end=" ")
    print('\n')

del rows[0]

time=0;
pulse_width=[];
trigger = False;

for row in rows:
    try:
        if(not trigger):
            if float(row[1]) > 2.0:
                trigger = True
                time=float(row[0])
        else:
            if float(row[1]) < 2.0:
                trigger = True
                pulse_width.append(float(row[0])-time)
    except:
        print("error: " +row[1])

print(pulse_width)

savefile = "scope_2_width.csv"
with open(savefile, 'w') as csvfile:
    csvwriter = csv.writer(csvfile)        # Create writer object
    csvwriter.writerow(pulse_width)              # Write multiple rows
