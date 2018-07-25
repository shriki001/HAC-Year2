#%%--------------------------------------------------------------------------%%#
# Write by Michael Shriki, LOGIN = michaelshr,
# and Kfir Matityahu, LOGIN = kfirma

#ex7.1


class ErrorDigit(Exception):
    pass


class ErrorSymmetric(Exception):
    pass


class ErrorEuler(Exception):
    pass


class ExceptEuler(Exception):
    pass


class ExceptList(Exception):
    pass


def check_symmetric(lst):

    newlist = []

    try:
        for x in lst:
            if not x.isdigit():
                raise ErrorDigit()
            else:
                newlist.append(int(x))

        start = 0
        end = int(len(lst) - 1)
        sum = newlist[start] + newlist[end]

    except ErrorDigit:
        print("Your List Is Not Digits!!")
        return

    return symmetric(newlist, start, end, sum)


def symmetric(lst, start, end, sum):

    try:
        if lst[start] + lst[end] != sum:
            raise ErrorSymmetric()
    except ErrorSymmetric:
        print("Your List Is Not Symmentric!!")
        return
    try:
        if start > end:
            raise ExceptList()
    except ExceptList:
        return True

    return symmetric(lst, start + 1, end - 1, sum)


def ex1():

    mylist = [x for x in input("Enter Yor List To Check:\n").split()]
    print("List result is: {}" .format(check_symmetric(mylist)))

#------------------------------------------------------------------------------
#ex7.2


def factorial(n):
    if n == 0:
        return 1
    else:
        return n * factorial(n - 1)


def func(num1, num2):
    return (num1**num2)/factorial(num2)


def calc(x, d, sum, index):
    try:
        if func(x, index) > d:
            raise ErrorEuler()
        else:
            raise ExceptEuler()

    except ErrorEuler:
        return sum
    except ExceptEuler:
        sum = sum + func(x, index)

    return calc(x, d, sum, index + 1)


def ex2():

    x = float(input("Enter X val: "))
    d = float(input("Enter D val: "))
    print("The sum is: {}".format(calc(x, d, 0, 0)))

#==============================================================================


def main():
    exercises = [ex1, ex2]
    menu = int(input("Enter Exercises num: (0 to Exit)\n"))
    while menu != 0:
        if 0 < menu <= 2:
            exercises[menu - 1]()
        else:
            print("This Exercises isn't exist!")
        menu = int(input("Enter Exercises num: (0 to Exit)\n"))


main()
