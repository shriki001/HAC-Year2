a = int(input("Enter your first number:"))
b = int(input("Enter your second number:"))
print("""‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
C A L C U L A T I O N S
‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
1. Addition
2. Subtraction
3. Multiplication
4.  Division  
5. Floor Division
6. Modulus
7. Exponent
‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
""")

f = int(input("Enter your choice [1‐7] :"))


def calc(num, x1, x2):
    if num == 1:
        return x1 + x2
    elif num == 2:
        return x1 - x2
    elif num == 3:
        return x1 * x2
    elif num == 4:
        return x1 / x2
    elif num == 5:
        return x1 // x2
    elif num == 6:
        return x1 % x2
    elif num == 7:
        return x1 ** x2
    else:
        print("Error")
        exit(1)


print("The result is:", calc(f, a, b))
