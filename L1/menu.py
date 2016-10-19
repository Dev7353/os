# from colorama import Fore
from helper import *
import sys

allowed = ['1', '2', '3', 'q']


def menu_options():
    print("1: Python Info\n")
    print("2: System Info\n")
    print("3: CPU Info\n")
    print("q: Quit Menu.py\n")


def pretty_print(d):

    for key in d.keys():
        output = key + "\t" + d[key]
        print(output.expandtabs(30))


def pretty_print_nested(d):
    for k in d.keys():
        pretty_print(d[k])
        print("")


def main():
    menu_options()
    while True:
        print("------------------------------------------\n")
        try:
            entry = str(
                input(
                    "Choose " +
                    #  Fore.RED +
                    str(allowed) +
                    #  Fore.WHITE +
                    ": "))

            assert entry in allowed

            if entry == '1':
                print("Python Version: " + python_info())
            elif entry == '2':
                pretty_print(system_info())
            elif entry == '3':
                pretty_print_nested(cpu_info())
            else:
                print("Program exit.")
                sys.exit()

        except AssertionError:
            print("Doesnt exist. Type in again.")

if __name__ == "__main__":
    main()
