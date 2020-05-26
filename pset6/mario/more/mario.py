# Problem Set 6
# mario/more
# Angus Leung
# May 4, 2020

from cs50 import get_int
while True:
    height = get_int("Height: ")
    if height in range(1, 9):
        break

for x in range(height):
    print(" " * (height - x - 1), end="")
    print("#" * (x + 1), end="")
    print("  ", end="")
    print("#" * (x + 1), end="")
    print()