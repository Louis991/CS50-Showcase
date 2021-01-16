from sys import argv, exit
import csv

# Command-line argument check.
if len(argv) != 3:
    print("Correct usage: python dna.py 'path of .csv database' 'path of DNA sequence .txt'")
    exit(1)

# Header = ["name", "AGATC", "TTTTTTCT", ...]
header = []

# data = [["Albus", "15", "49", ...], ["Cedric", "31", "21", ...] ... ]
data = []

# Open CSV file and extract its data into the previous lists.
with open(argv[1], 'r') as csv_file:
    csv_reader = csv.reader(csv_file)
    lineCount = 0
    # Each row is a list of string elements.
    for row in csv_reader:
        # Extract data from the header.
        if lineCount == 0:
            header.extend(row)
            lineCount += 1
        # Extract other data, row by row, each row corresponding to each person.
        else:
            data.append(row)
            lineCount += 1

# Open sequence text file and store it into memory.
with open(argv[2], 'r') as text:
    sequence = text.read()

# Convert numbers in data from strings to integers.
for person in data:
    for x in range(1, len(header), 1):
        person[x] = int(person[x])

# Length of the header minus "name".
STRnum = len(header) - 1

# Loops through each person's data to determine if there is a match.
for person in data:
    count = 0
    for head in range(1, len(header), 1):
        # If, for example, AGATCAGATCAGATC (header[head] * 3) exists in the sequence and is the highest consecutive count for that STR, AGATC (header[head] * 1) and AGATCAGATC (header[head] * 2) are excluded when determining a match.
        if (header[head] * person[head] in sequence) and (header[head] * (person[head] + 1) not in sequence):
            count += 1
        # If there's a match.
        if count == STRnum:
            print(person[0])
            exit(0)

# If there's not a match.
print("No match")