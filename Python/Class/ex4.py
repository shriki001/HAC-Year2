#%%--------------------------------------------------------------------------%%#
#ex4

basket = ['Tomatoes', 'Cucumbers', 'Peppers', 'Onions', 'Potatoes', 'Carrots',
          'Zucchinis', 'Cabbages', 'Lettuces', 'Eggplants']


def make_list(name, address, **args):
    print("Your Name is: {}".format(name))
    print("Your Addresses is: {}".format(address))
    print("you'v ordered the following:")
    keys = sorted(args.items())
    for kw in keys:
        if 0 not in kw:
            print(list(kw))


name = input("Enter your Name: ")
address = input("your Address: ")
print("This is the available Vegetables list:")
for i, a in enumerate(basket):
    print(i, a)
lst = [int(x) for x in input("Enter your Vegetables capacity (mast "
                             "type 10 according to the list above, 0 for "
                             "non)\n").split()]
make_list(name, address, Tomatoes=lst[0], Cucumbers=lst[1], Peppers=lst[2],
          Onions=lst[3], Potatoes=lst[4], Carrots=lst[5], Zucchinis=lst[6],
          Cabbages=lst[7], Lettuces=lst[8], Eggplants=lst[9])

#%%--------------------------------------------------------------------------%%#
