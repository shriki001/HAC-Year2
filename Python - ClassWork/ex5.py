#%%--------------------------------------------------------------------------%%#
#ex5


class Product(object):
    def __init__(self, item_name, item_id, curr_storage, min_storage, item_price):
        self.m_name = item_name
        self.m_id = item_id
        self.m_currStorage = curr_storage
        self.m_minimalStorage = min_storage
        self.m_price = item_price

    def get_name(self):
        return self.m_name

    def get_id(self):
        return self.m_id

    def get_curr_storage(self):
        return self.m_currStorage

    def get_minimum_store(self):
        return self.m_minimalStorage

    def get_price(self):
        return self.m_price

    def add_storage(self, quantity):
        self.m_currStorage += quantity

#==============================================================================#


class Products(object):
    def __init__(self):
        self.m_items = []
        self.m_items.append(Product("Bread", 100001, 10, 10, 5.90))
        self.m_items.append(Product("Milk", 100002, 20, 14, 7.30))
        self.m_items.append(Product("Orange juice", 100003, 8, 5, 6.60))
        self.m_items.append(Product("Cigarettes", 100004, 20, 20, 21.50))
        self.m_items.append(Product("Rise", 100005, 5, 3, 7.00))
        self.m_items.append(Product("Chocolate", 100006, 10, 7, 4.90))
        self.m_items.append(Product("Pasta", 100007, 10, 8, 10.50))
        self.m_items.append(Product("Cottage", 100008, 7, 15, 100.00))

    def __setItem__(self, item, value):
        self.m_items[item] = value

    def add_items(self, item_id, quantity):
        for item in self.m_items:
            if item.get_id() == item_id:
                item.add_storage(quantity)
                break

    def sell_items(self, item_name, quantity):
        for item in self.m_items:
            if item.get_name() == item_name:
                item.add_storage(-quantity)
                break

    def print_available(self):
        for item in self.m_items:
            print("Item name = {} | Item id = {} | has in store = {}"
                  .format(item.get_name(), item.get_id(),
                          item.get_curr_storage()))


def check(items_list):
    for item in items_list.m_items:
        if item.get_curr_storage() < item.get_minimum_store():
            print("item Name = {} | Item id = {} | need to order = {}"
                  .format(item.get_name(), item.get_id(),
                          int(item.get_minimum_store()) -
                          item.get_curr_storage()))


product_list = Products()


def unit_test(items_list):
    while 1:
        choice = int(input("Please Enter a function to choose:\n"))
        if choice == 0:
            break
        elif choice == 1:
            items_list.print_available()
        elif choice == 2:
            check(items_list)
        elif choice == 3:
            id_item = int(input("Please Enter a product id:\n"))
            to_order = int(input("How many to order:\n"))
            items_list.add_items(id_item, to_order)
        elif choice == 4:
            item_name = input("Please Enter a product name:\n")
            to_sell = int(input("How many to sell:\n"))
            items_list.sell_items(item_name, to_sell)
        else:
            print("Unknown request")


unit_test(product_list)

#%%--------------------------------------------------------------------------%%#
