from cs50 import get_string
import re

text = get_string("Text: ")

# https://stackoverflow.com/questions/51560867/how-to-remove-all-non-alphabetic-characters-from-a-string
# first argument r'[^a-zA-Z ]' r = regular expression pattern, ^ = not, a-zA-Z lower and upper and spaces
# second argument what to replace with, in this case: ''
# third argument is string input
sentences = len(re.findall(r'[.!?]', text))
text = re.sub(r'[^a-zA-Z ]', '', text)
letters = re.sub(r'[^a-zA-Z]', '', text)

# Create a list of text split between spaces
text = text.split(" ")
wordcount = len(text)

index = round(0.0588 * (len(letters) * 100 / wordcount) - 0.296 * (sentences * 100 / wordcount) - 15.8)

if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")
