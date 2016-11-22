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


def increment():
    global global_var
    tempvar = global_var
    time.sleep(0.001)  # Sleep to show race conditions
    global_var = tempvar + 1


def race_lock(n_threads, n_loops):
    exc = global_var + n_threads * n_loops
    for i in range(n_threads):
        for i in range(n_loops):
            _thread.start_new_thread(increment, ())
    cur = global_var
    return (exc, cur)


class race_helper(threading.Thread):
    # Initialize the thread
    def __init__(self, n_loops):
        threading.Thread.__init__(self)
        self.n_loops = n_loops

    # Run increment function
    def run(self):
        for i in range(self.n_loops):
            increment()


def race(n_threads, n_loops):
    exc = global_var + n_threads * n_loops
    threads = []
    # Consecutive for loops to guarantee sequential procedure
    for i in range(0, n_threads):
        threads.append(race_helper(n_loops))
    for i in range(0, n_threads):
        threads[i].start()
    for i in range(0, n_threads):
        threads[i].join()
    cur = global_var
    return (exc, cur)


def main():
    print(global_var)
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
    # a = race_lock(threads, loops)
    a = race(threads, loops)
    # time.sleep(1)
    print(a)

if __name__ == "__main__":
    main()
