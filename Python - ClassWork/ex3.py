#%%--------------------------------------------------------------------------%%#
#ex3.1

lst1 = [int(x) for x in input("enter first series").split()]
lst2 = [int(x) for x in input("enter second series").split()]
if [x**2 for x in lst1] == lst2:
    m_list = [x + y for x, y in zip(lst1, lst2)]
    print(m_list)

################################################################################
#ex3.2

lst3 = [x for x in input("enter series").split()]
print(list(filter(lambda x: x.isdigit(), lst3)))

################################################################################
#ex3.3

lst4 = []
for x in range(1, 200):
    if x % 35 == 0:
        lst4 += [x]
for i, a in enumerate(lst4):
    print(i + 1, a)

################################################################################
#ex3.4

def add(x, y): return x + y
def sub(x, y): return x - y
def mul(x, y): return x * y
def div(x, y): return x / y
def fdiv(x, y): return x // y
def mod(x, y): return x % y
def exp(x, y): return x ** y

funcs = [add, sub, mul, div, fdiv, mod, exp]

var1 = int(input("Enter your first number:"))
var2 = int(input("Enter your second number:"))
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

value = list(map(lambda x: x(var1, var2), funcs))
print(value)

#%%--------------------------------------------------------------------------%%#
