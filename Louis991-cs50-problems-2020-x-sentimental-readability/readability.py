# Getting the text input.
text = input("Text: ")

# Variables used when extracting information from the text.
letterCount = 0
spaceCount = 1
sentenceCount = 0


# Extracting information from the text.
for letter in text:
    # The number of letters
    if (letter >= 'A' and letter <= 'Z') or (letter >= 'a' and letter <= 'z'):
        letterCount += 1
    # The number of spaces
    if (letter == ' '):
        spaceCount += 1
    # The number of sentences
    if (letter == '.' or letter == '!' or letter == '?'):
        sentenceCount += 1

# Calculating the index.
l = (letterCount / spaceCount) * 100
s = (sentenceCount / spaceCount) * 100
index = round(0.0588 * l - 0.296 * s - 15.8)

# Output
if (index < 1):
    print("Before Grade 1")
elif (index >= 16):
    print("Grade 16+")
else:
    print(f"Grade {index}")
