from sys import exit
from cs50 import get_int


def main():
    h = 0
    while h < 1 or h > 8:
        h = get_int("Height: ")
    pyramid(h)
    exit(0)
    

def pyramid(height):
    space = " "
    hash = "#"
    for n in range(height):
        print(f"{space * (height - 1 - n)}{hash * (n + 1)}  {hash * (n + 1)}")
    return True


main()
