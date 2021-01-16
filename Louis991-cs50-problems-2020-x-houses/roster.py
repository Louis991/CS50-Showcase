from sys import argv, exit
import cs50

# Command-line-check
if len(argv) != 2:
    print("Correct usage: python import.py 'name of house'")
    exit(1)

# Setup the database connection
db = cs50.SQL("sqlite:///students.db")

# db.execute returns a list of dict objects, each one corresponds to a student.
rows = db.execute("SELECT * FROM students WHERE (house == ?) ORDER BY last ASC, first ASC", argv[1])

# Prints student data.
for row in rows:
    if row["middle"] != None:
        print("{0} {1} {2}, born {3}".format(row["first"], row["middle"], row["last"], row["birth"]))
    else:
        print("{0} {1}, born {2}".format(row["first"], row["last"], row["birth"]))