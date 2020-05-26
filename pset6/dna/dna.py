# CS 50 Problem Set 6
# DNA
# Angus Leung
# May 6, 2020

from sys import argv
import csv
import re

# Confirms correct usage
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit()


# Import sequence file into string "sequence"
with open(argv[2], 'r') as sequence_file:
    for line in sequence_file:
        sequence = str(line)


# Open and work with database (dna) file
with open(argv[1], 'r') as dna_file:
    # Function to input STR (dna keys) to a list
    reader = csv.reader(dna_file)
    keys = next(reader)
    
    # Remove Names Column
    keys.pop(0)
    
    # Define an ordered, changeable list for storing sequence STR
    STRlist = []   

    for i in range(len(keys)):
        # Using re.compile to create the search query beforehand
        STR = re.compile(r'''
                    (?:%s)             # Non-capturing group over my string variable 
                    +'''               # One or more occurrences
                         % keys[i]     # Variable for %s
                         , re.VERBOSE)
             
        # re.search only gives you the first instance, not the largest value in the string
        # re.findall will give me a list of all matches
        STRfind = STR.findall(sequence)     # Example Output: <re.Match object; span=(52, 56), match='AATG'>
       
        # If there's no matches, then it'll give an attribute error (as it's defined as 'NoneType')
        # Create a try/except to move past AttributeError
        try:
            # Function to count # of repeats
            j = 0
            # while STRfind exists (is not empty)
            while STRfind:
                if max(STRfind) == keys[i] * j:
                    STRlist.append(str(j))
                    break
                else:
                    j += 1

        except AttributeError:
            # If there's a 'NoneType' AttributeError, it means the STR was not found (= 0)
            STRlist.append(str("0"))
            # After the Try/Except above, a count of all STR keys found in sequence file will be stored in list STRlist
            
    # Continue reading in database (dna) file 
    for row in dna_file:
        # Remove \n from the row we're reading
        row = row.strip()
        
        # row is a string, convert to a list called Person
        Person = row.split(',')
        
        # Remove names from Person list. Compare both STR lists (database vs sequence)
        if Person[1:len(STRlist) + 1] == STRlist:
            print(Person[0])
            exit()
        else:
            continue
    
    # If the program runs to here, there is no match
    print("No Match")