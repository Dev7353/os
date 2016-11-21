#!usr/bin/env python3

import getopt
import sys
import _thread
import threading
import time

global_var = 600


def usage():
    '''
    :return: The help string
    '''
    return """Usage: racecondition.py [OPTION]

         -h, --help         	Displays this help
         -t, --threads       	Sets how often the global variable threads
                                gets incremented (Default: 10)
         -l, --loops          	Amount of threads to be created"""


def increment(n_loops):
    global global_var
    for i in range(0, n_loops):
        global_var += 1


def race_lock(n_threads, n_loops):
    exc = global_var + n_threads * n_loops
    for i in range(0, n_threads):
        _thread.start_new_thread(increment, (n_loops,))
    cur = global_var
    return (exc, cur)


def race(n_threads, n_loops):
    exc = global_var + n_threads * n_loops
    threads = []
    for í in range(0, n_threads):
        t = threading.Thread(target=increment, args=(n_loops,))
        threads.append(t)
        t.start()
    cur = global_var
    return (exc, cur)


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
    # race_lock(threads, loops)
    race(threads, loops)
    time.sleep(1)
    print(global_var)

if __name__ == "__main__":
    main()
