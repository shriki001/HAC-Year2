#%%--------------------------------------------------------------------------%%#
#ex6
# Write by Michael Shriki, LOGIN = michaelshr,
# and Kfir Matityahu, LOGIN = kfirma


import csv
from collections import defaultdict

#=====================================================================================


def file_read_from_head(file_name, lines_num):
    with open(file_name) as f:
        for _ in range(lines_num):
            line = f.readline()
            print(line, end='')

#=====================================================================================


def is_exist(file_name, city):
    if city in open(file_name).read():
        print("True")
    else:
        print("False")

#=====================================================================================


def list_of_field(field):
    columns = defaultdict(list)  # each value in each column is appended to a list

    with open('us_postal_codes.csv') as f:
        reader = csv.DictReader(f)  # read rows into a dictionary format
        for row in reader:  # read a row as {column1: value1, column2: value2,...}
            for (k, v) in row.items():  # go over each column name and value
                columns[k].append(v)  # append the value into the appropriate list
                # based on column name k

    return columns[field]

#=====================================================================================


def write_uniqe_states():
    output_file = open('unique_names.txt', 'w')
    list_states = list_of_field('State')
    output_file.write("before filter:%d\n" % len(list_states))
    sorted_states = sorted(set(list_of_field('State')))
    output_file.write("after filter:%d\n" % len(sorted_states))
    for state in sorted_states:
        output_file.write("%s\n" % state)

#1)


numOfLines = int(input("number of lines to remove (from begin):\n"))
file_read_from_head('us_postal_codes.csv', numOfLines)

#2)
city2find = input("check if city exist:\n")
is_exist('us_postal_codes.csv', city2find)

#3)
postal2find = input("find Postal code exist:\n")
is_exist('us_postal_codes.csv', postal2find)

#4)
write_uniqe_states()
