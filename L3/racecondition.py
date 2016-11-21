#!usr/bin/env python3

import getopt
import sys
import _thread
import time


def usage():
    '''
    :return: The help string
    '''
    return """Usage: racecondition.py [OPTION]

         -h, --help         	Displays this help
         -t, --threads       	Sets how often the global variable threads
                                gets incremented (Default: 10)
         -l, --loops          	Amount of threads to be created"""


def race_lock(n_threads, n_loops):
    return (exc, cur)


def race(n_threads, n_loops):
    print("Hallo")


def main():
    threads = 40
    loops = 10
    try:
        options, args = getopt.getopt(
            sys.argv[1:], 'tlh', ['threads', 'loops', 'help'])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err)  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    for opt, arg in options:
        if opt in ('-t', '--output'):
            if arg is None:
                break
            else:
                threads = arg
        if opt in ('-l', '--loops'):
            if arg is None:
                break
            else:
                loops = arg
        if opt in ('-h', '--help'):
            print(usage())
            break
    try:
        _thread.start_new_thread(race, (threads, loops))
    except:
        print("Error!")
    time.sleep(1)

if __name__ == "__main__":
    main()
