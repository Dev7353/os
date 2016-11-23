#!usr/bin/env python3

import getopt
import sys
import threading  # Use this module because _thread does not have join method
import time

global_var = 600
threads = 0
threadLock = threading.Lock()
lockcount = []
delay = 0.0001


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
    global delay
    tempvar = global_var
    time.sleep(delay)  # Sleep to show race conditions
    global_var = tempvar + 1


def race_lock(n_threads, n_loops):
    exc = global_var + n_threads * n_loops
    threads = []
    # Consecutive for loops to guarantee sequential procedure
    for i in range(0, n_threads):
        threads.append(init_thread(n_loops, True))
    for i in range(0, n_threads):
        threads[i].start()
    for i in range(0, n_threads):
        threads[i].join()
    cur = global_var
    return (exc, cur)


class init_thread(threading.Thread):
    # Initialize the thread, overwrite to add arguments
    def __init__(self, n_loops, locked):
        threading.Thread.__init__(self)
        self.n_loops = n_loops
        self.locked = locked

    # Overwrite to run increment function
    def run(self):
        if self.locked is True:
            for i in range(self.n_loops):
                threadLock.acquire()
                increment()
                threadLock.release()
        else:
            for i in range(self.n_loops):
                increment()


def race(n_threads, n_loops):
    exc = global_var + n_threads * n_loops
    threads = []
    # Consecutive for loops to guarantee sequential procedure
    for i in range(0, n_threads):
        threads.append(init_thread(n_loops, False))
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
    gil = False
    mutex = False
    peters = False
    output = ""
    try:
        options, args = getopt.getopt(
            sys.argv[1:], 'tlhgpm', ['threads', 'loops', 'help', 'gil', 'peterson', 'mutex'])
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
        if opt in ('-g', '--gil'):
            gil = True
        if opt in ('-h', '--help'):
            print(usage())
            break
    if gil is True:
        output = str(race_lock(threads, loops))
    else:
        output = str(race(threads, loops))
    # time.sleep(1)
    print(output)

if __name__ == "__main__":
    main()

# https://www.tutorialspoint.com/python/python_multithreading.htm
