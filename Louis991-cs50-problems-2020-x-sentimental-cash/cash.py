from cs50 import get_float

# Amount owed and counter for the minimum number of coins required.
owed = 0
count = 0

# Getting user input.
while owed <= 0:
    owed = get_float("Enter the amount owed: ")

# Converting the amount to cents
owed = round(owed * 100)

# Determining the min. amount of coins
if owed >= 25:
    while owed >= 25:
        owed -= 25
        count += 1

if owed >= 10:
    while owed >= 10:
        owed -= 10
        count += 1

if owed >= 5:
    while (owed >= 5):
        owed -= 5
        count += 1

if owed >= 1:
    while (owed >= 1):
        owed -= 1
        count += 1

# Printing the output
print(f"{count}")