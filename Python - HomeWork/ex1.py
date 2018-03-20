# OS2 Python Programming 2018b
#
# File:ex1.py
# ==============================================================================
# Writen by: Michael Shriki login = michaelshr
#            Kfir Matityahu login = kfirma
#
# Run: ex1.py using Python interpreter
#%%--------------------------------------------------------------------------%%#

"""
    Exercises 1: prints all numbers that their sum of digits cubed are equal to
    themselves.
"""


def ex1():
    for x in range(100, 500):
        if x == (x % 10)**3 + ((x//10) % 10)**3 + (x // 100)**3:
            print(x)


################################################################################

"""
    Exercises 2: Solves the question - if you buy 100 tickets for 400$, when each
    ticket costs either 3$,10$ or 15$
        How many tickets of each type did you buy?
"""


def ex2():

    def find(tickets, budget):
        """Finds, for a given tickets amount and budget how many tickets of each
         type you bought"""

        for x in range(tickets):
            for y in range(tickets):
                for z in range(tickets):
                    if ((3 * x) + (10 * y) + (15 * z) == budget) & \
                            ((x + y + z) == tickets):
                        return [str(x) + " tickets for 3$", str(y) +
                                " tickets for 10$", str(z) + " tickets for 15$"]
    for found in find(100, 400):
        print(found)


################################################################################

"""
    Exercises 3: Plays with user 7 boom! where starting player is selected 
        randomly for extra fun.
        Each player types the next number, but if the number is a multiple of 7 
        or has 7 in it then player must type
        "Boom" instead of the number. Also correct order of numbers must be 
        kept. 
        Player is winner if was able to get to 30.
"""


def ex3():
    def check(num): return num % 7 == 0
    won = True
    for turn in range(1, 31):
        if turn % 2 == 1:
            if check(turn):
                print("Computer: Boom!")
            else:
                print("Computer: ", turn)
        else:  # odd turns are of the user
            user_choice = input("User: ")
            if user_choice == "Boom!" and check(turn):
                pass
            elif int(user_choice) == turn:
                pass
            else:
                print("You Lost!")
                won = False
                break
    if won:
        print("You Won!")


################################################################################

"""
Exercises 4: Implement a stack using list with operations:
    i - insert (push) to stack
    e - eject(pop) from stack
    p - print the stack

    When stack is empty, print will print "The Stack is Empty!"
    If stack is empty and e (pop) is called, the function will exit.
"""


def ex4():
    my_stack = []
    user_input = input("Enter a command (i: insert, e: Eject, p: print): ")

    while 1:
        if user_input == "i":
            my_stack.append(input("Enter string: "))
        elif user_input == "e":
            if not my_stack:
                print("The Stack is Empty!")
                break
            else:
                my_stack.pop()
        elif user_input == "p":
            if not my_stack:
                print("The Stack is Empty!")
                pass
            else:
                for i, a in enumerate(my_stack):
                    print(i, a)
        else:
            print("This key isn't exist!")
        user_input = input("Enter a command (i: insert, e: Eject, p: print): ")


#-------------------- Main Code for checking the program ----------------------#

def main():
    """
    Prompts a selection for user to choose between the 4 Exercises.
    1,2,3,4 corresponds to the question numbers.
    0 will exit the program.
    For all other inputs, the program will display a retry message.
    """
    quest = [ex1, ex2, ex3, ex4]
    menu = int(input("Enter Exercises num: (0 to Exit)\n"))
    while menu != 0:
        if 0 < menu <= 4:
            quest[menu-1]()
        else:
            print("This Exercises isn't exist!")
        menu = int(input("Enter Exercises num: (0 to Exit)\n"))


main()

#%%--------------------------------------------------------------------------%%#
