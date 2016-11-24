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
    time.sleep(delay)  # Sleep to give control to different thread
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


def race_p(n_threads, n_loops):
    exc = global_var + n_threads * n_loops
    threads = []
    level = []
    last_to_enter = []
    # Set up flags for Peterson algorithm
    for i in range(n_threads):
        level.append(-1)
    for i in range(n_threads - 1):
        last_to_enter.append(0)
    for i in range(n_threads):
        t = threading.Thread(target=peterson, args=(n_loops, n_threads, level, last_to_enter, i))
        threads.append(t)
    for i in range(n_threads):
        threads[i].start()
    for i in range(n_threads):
        threads[i].join()
    cur = global_var
    return (exc, cur)


def peterson(n_loops, n_threads, level, last_to_enter, i):
    k = 1
    for l in range(n_threads - 1):
        level[i] = l
        last_to_enter[l] = i
        while(last_to_enter[l] == i and level[k] >= l):
            if(k != i):
                time.sleep(0.1)
        k += 1
    for x in range(n_loops):
        increment()
    print(global_var)
    level[i] = -1


def main():
    print(global_var)
    threads = 40
    loops = 10
    gil = False
    mutex = False
    peterson = False
    output = ""
    try:
        options, args = getopt.getopt(
            sys.argv[1:], 'tlhgpm', ['threads', 'loops', 'help', 'gil',
                                     'peterson', 'mutex'])
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
        if opt in ('-p', '--peterson'):
            peterson = True
        if opt in ('-m', '--mutex'):
            mutex = True
        if opt in ('-h', '--help'):
            print(usage())
            break
    if gil is True:
        output = str(race_lock(threads, loops))
    elif peterson is True:
        output = str(race_p(threads, loops))
    elif mutex is True:
        pass
    else:
        output = str(race(threads, loops))
    print(output)

if __name__ == "__main__":
    main()

# https://www.tutorialspoint.com/python/python_multithreading.htm
