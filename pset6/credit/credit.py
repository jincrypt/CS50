# Problem Set 6
# credit
# Angus Leung
# May 4, 2020
from cs50 import get_int


def main():
    card = get_card()

    confirm_card(card)
    

def get_card():
    # Loops and ensures we get positive numbers
    while True:
        card = get_int("Number: ")
        if (card > 0):
            return card


def confirm_card(card):
    # converts to a string so we can work with individual values
    strcard = str(card)
    AMEX = 0
    MC = 0
    VISA = 0

    LuhnTest1 = 0
    LuhnTest2 = 0
    LuhnTest3 = 0

    # Card length check
    # AMEX 15 / Starts with 34 or 37
    if len(strcard) == 15 and int(strcard[0]) == 3 and int(strcard[1]) is (4 | 7):
        AMEX = 1
    # MC Length 16 / Starts with 51 52 53 54 55
    elif len(strcard) == 16 and int(strcard[0]) == 5 and int(strcard[1]) in range(1, 6):
        MC = 1
    # VISA Length 13 or 16, starts with 4
    elif (len(strcard) == 13 or len(strcard) == 16) and int(strcard[0]) == 4:
        VISA = 1
    else:
        print("INVALID")
        exit(2)

    # Confirm Luhn's Algorithm
    # LuhnTest1: Multiply every other digit by 2, starting with second to last digit
    i = len(strcard) - 2
    while (i >= 0):
        LuhnTest1 += (int(strcard[i]) * 2) % 10 + (int(strcard[i]) * 2 // 10) % 10
        i -= 2
    
    # LuhnTest2: Add sum of digits that weren't multiplied by 2
    j = len(strcard) - 1
    while (j >= 0):
        LuhnTest2 += int(strcard[j]) 
        j -= 2
        
    # LuhnTest3: If total last digit is 0, number is valid (mod 10 = 0)
    LuhnTest3 = (LuhnTest2 + LuhnTest1) % 10
    if LuhnTest3 == 0:
        if AMEX == 1:
            print("AMEX")
        elif MC == 1:
            print("MASTERCARD")
        elif VISA == 1:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")
        
        
# Executes main function
main()
