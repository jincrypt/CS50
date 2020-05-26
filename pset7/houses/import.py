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
    
db = SQL("sqlite:///students.db") 

# Deletes table if it exists
db.execute("DROP TABLE IF EXISTS students;")

# Create table headers
db.execute("CREATE TABLE students(id INTEGER PRIMARY KEY AUTOINCREMENT,first VARCHAR(255),middle VARCHAR(255),last VARCHAR(255),house VARCHAR(255),birth INTEGER)")

# Import character file into string "characters"
with open(argv[1], 'r') as character_file:
    reader = csv.DictReader(character_file)
    for line in reader:
        fullname = (line['name'].split(" "))
        if len(fullname) == 2:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?,?,?,?,?)", 
                       (fullname[0], None, fullname[1], line['house'], line['birth']))
        if len(fullname) == 3:  
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?,?,?,?,?)",
                       (fullname[0], fullname[1], fullname[2], line['house'], line['birth']))