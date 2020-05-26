# CS50 PSET7 Houses
# Angus Leung
# May 12, 2020

from sys import argv
import csv
from cs50 import SQL

# Confirms correct usage
if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit()

house = argv[1]

db = SQL("sqlite:///students.db") 
query = db.execute("SELECT first, middle, last, birth FROM students WHERE house = (?) ORDER BY last, first", (house))

for row in query:
    if row['middle'] != None:
        middle = row['middle']
        print("{} {} {}, born {}".format(row['first'], row['middle'], row['last'], str(row['birth'])))

    else:
        print("{} {}, born {}".format(row['first'], row['last'], str(row['birth'])))
