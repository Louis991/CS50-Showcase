from sys import argv, exit
import csv
import cs50
import re

# Command-line argument check.
if len(argv) != 2:
    print("Correct usage: python import.py 'path of .csv file' ")
    exit(1)

# Setup the database connection
db = cs50.SQL("sqlite:///students.db")

# Create a regex object. This stores the pattern of each student's possible name.
namePattern = re.compile(r'(\w{1,})\s(\w{1,}-\w{1,}|\w{1,})\s?(\w{1,})?')

# Open CSV file.
with open(argv[1], 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:

        # Pass the current name into the regex object's search() method. This returns a match object.
        runRe = namePattern.search(row["name"])
 
        # Use the match object's groups() method to determine the type of name and to grab each part of it.

        # If name -> first name + last name.
        if runRe.group(3) == None:
            # Write the current row to the DB.
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", 
                       runRe.group(1), None, runRe.group(2), row["house"], row["birth"])

        # If name -> first name + middle name + last name.
        if runRe.group(3) != None:
            # Write the current row to the DB.
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", 
                       runRe.group(1), runRe.group(2), runRe.group(3), row["house"], row["birth"])