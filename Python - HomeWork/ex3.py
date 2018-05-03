# OS2 Python Programming 2018b
#
# File:ex3.py
# ==============================================================================
# Writen by: Michael Shriki login = michaelshr
#            Kfir Matityahu login = kfirma
#
# Run: ex3.py using Python interpreter
#%%--------------------------------------------------------------------------%%#


import random
import os


class Person(object):
    def __init__(self, name, last_name, address, id):
        self.m_name = name
        self.m_lastName = last_name
        self.m_address = address
        self.m_id = id


class BankAccount(object):
    def __init__(self, account_number, details: Person, balance):
        self.m_accountNumber = account_number
        self.m_details = details
        self.m_balance = balance
        self.m_commission = 5
        self.history_file_name = "history.txt"
        self.init2()

    def __del__(self):
        os.remove("history.txt")

    def init2(self):
        f = open(self.history_file_name, "w")
        f.write("New Account , Balance: {}\n".format(self.m_balance))
        f.close()

    def deposit_money(self, to_deposit):
        self.m_balance += (to_deposit - self.m_commission)
        # f = open("history.txt", "a")
        f = open(self.history_file_name, "a")
        f.write("Deposit: {} , Balance: {}\n".format(to_deposit, self.m_balance))
        f.close()

    def withdraw_money(self, to_withdraw):
        f = open(self.history_file_name, "a")
        if self.m_balance >= to_withdraw:
            self.m_balance -= (to_withdraw + self.m_commission)
            f.write("Withdraw: {} , Balance: {}\n".format(to_withdraw,
                                                                        self.m_balance))
        else:
            print("Cannot withdraw")
            f.write("Withdraw: 0 , Balance: {}\n".format(self.m_balance))

        f.close()

    def print(self):
        print("Account : {}".format(self.m_accountNumber))
        print("Name    : {} {}".format(self.m_details.m_name, self.m_details.m_lastName))
        print("Balance : {}".format(self.m_balance))

    def print_history(self):
        f = open(self.history_file_name, "r")
        lines = f.readlines()
        for line in lines:
            print(line, end="")
        f.close()


class StudentAccount(BankAccount):
    def __init__(self, account_number, details: Person, balance, academy):
        super().__init__(account_number, details, balance)
        self.m_academy = academy
        self.m_commission = 3

    def print(self):
        print("Student account : {}".format(self.m_accountNumber))
        print("Name            : {} {}".format(self.m_details.m_name, self.m_details.m_lastName))
        print("Balance         : {}".format(self.m_balance))

##############################################################


def create_account():
    account_name = input("Please enter your name:\n")
    account_last_name = input("Please enter your last name:\n")
    address = input("What is your address?\n")
    account_id = int(input("What is your ID?\n"))
    account_balance = int(input("What is your balance?\n"))
    account_type = (input("What is the account type (Student/Bank)?\n"))
    account_num = random.randint(10000, 99999)

    new_account = 0
    account_details = Person(account_name, account_last_name, address, account_id)
    if account_type == "Student":
        new_account = StudentAccount(account_num, account_details ,account_balance, "HAC")
    elif account_type == "Bank":
        new_account = BankAccount(account_num, account_details, account_balance)

    while True:
        options = ["deposit", "withdraw", "print", "exit", "show history"]
        print("Choose an option from below: ")
        for i, a in enumerate(options):
            print(i+1, a)

        choice = int(input("\n"))

        if choice == 1:
            to_deposit = int(input("How much to deposit? "))
            new_account.deposit_money(to_deposit)

        elif choice == 2:
            to_withdraw = int(input("How much to withdraw? "))
            new_account.withdraw_money(to_withdraw)

        elif choice == 3:
            new_account.print()

        elif choice == 4:
            break

        elif choice == 5:
            new_account.print_history()

#/////////////////////////////////////////////////////////////////////////////#
# the main program


create_account()
