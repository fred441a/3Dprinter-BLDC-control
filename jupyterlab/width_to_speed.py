import csv



savefile = "scope_0_speed.csv"
with open(filename, 'w') as csvfile:
    csvwriter = csv.writer(csvfile)        # Create writer object
    csvwriter.writerow([])              # Write multiple rows
