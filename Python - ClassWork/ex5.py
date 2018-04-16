#%%--------------------------------------------------------------------------%%#
#ex5

import random


class Product(object):
    def __init__(self, id_item, class_name, in_store, price=10.00):
        self.m_id = id_item
        self.m_class = class_name
        self.m_inStorage = in_store
        self.m_minimalStore = 100
        self.m_price = price

    def get_in_storage(self):
        return self.m_inStorage

    def get_class(self):
        return self.m_class

    def get_minimum(self):
        return self.m_minimalStore

    def get_price(self):
        return self.m_price

    def get_id(self):
        return self.m_id

    def print1(self):
        print("Product id = {} ,price = {}, in storge have = {} , mimnum = {},"
              " class = {}"
              .format(self.m_id, self.m_price, self.m_inStorage,
                      self.m_minimalStore, self.m_class))


class Products(object):
    def __init__(self):
        self.m_items = []

    def add_item(self, item):
        self.m_items.append(item)

    def __removeItem__(self, item):
        self.m_items.remove(item)

    def __getItem__(self, item):
        return self.m_items[item]

    def __setItem__(self, item, value):
        self.m_items[item] = value

    def print(self):
        for item in self.m_items:
            item.print1()


def check(items_list):
    for item in items_list.m_items:
        if item.get_in_storage() < item.get_minimum():
            print("Item id = {} | need to order = {}"
                  .format(item.get_id(), int(item.get_minimum()) -
                          item.get_in_storage()))


def check_class(items_list, class_name):
    for item in items_list.m_items:
        if item.get_class() == class_name:
            print("Item id = {} | price = {}".format(item.get_id(),
                                                     item.get_price()))


def check_code(items_list, item_id):
    for item in items_list.m_items:
        if item.get_id() == item_id:
            return item.get_price()


def check_storage(items_list, k):
    count = 0
    for item in items_list.m_items:
        if item.get_in_storage() > k:
            count += 1
    return count


classes = ["Furniture", "Food", "", "", ""]

p = Products()
for i in range(1, 100):
    p.add_item(Product(i+1000, classes[random.randint(0, 4)], random
                       .randint(10, 200), random.randint(1, 10)*1.5))

check(p)

print("#########################################\n")
check_class(p, "Furniture")

print("#########################################\n")
print("price of item with id {} is {} ".format(1005, check_code(p, 1005)))

print("#########################################\n")
c = 100
print("number of item with more than {} in storage : {}"
      .format(c, check_storage(p, c)))

print("#########################################\n")
p.print()

#%%--------------------------------------------------------------------------%%#
