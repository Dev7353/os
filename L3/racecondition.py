#!usr/bin/env python3

import getopt
import sys
import threading  # Use this module because _thread does not have join method
import time
import timeit

global_var = 600
threadcount = 0
threadLock = threading.Lock()
lockcount = []
delay = 0.0001


def usage():
    '''
    :return: The help string
    '''
    return """Usage: racecondition.py [OPTION]
         This program increments a global variable a certain amount of times.
         Without any given parameter, it runs it without any locks with
         40 threads and 10 loops.

         -h, --help             Displays this help
         -t, --threads          Sets how often the global variable threads
                                gets incremented (Default: 10)
         -l, --loops            Amount of threads to be created
         -g, --gil              Runs the program with GIL
         -p, --peterson         Runs the program with the peterson algorithm
         -m, --mutex            Runs the program with mutex"""


def increment():
    '''
    Increments the global variable and sleeps for a delay to give control to
    different thread
    '''
    global global_var
    global delay
    tempvar = global_var
    time.sleep(delay)  # Sleep to give control to different thread
    global_var = tempvar + 1


def race_lock(n_threads, n_loops):
    '''
    Executes the incrementing with GIL
    '''
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

    '''
    Helper class to run incrementing with or without lock.
    '''
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
    '''
    Executes the incrementing without locks
    '''
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
    '''
    Executes the incrementing with Peterson Algorithm
    '''
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
        threads.append(threading.Thread(target=peterson, args=(n_loops,
                                                               n_threads,
                                                               level,
                                                               last_to_enter,
                                                               i)))
    for i in range(n_threads):
        threads[i].start()
    for i in range(n_threads):
        threads[i].join()
    cur = global_var
    return (exc, cur)


def peterson(n_loops, n_threads, level, last_to_enter, i):
    '''
    Helper function to execute peterson algorithm.
    '''
    k = 1
    for l in range(n_threads - 1):
        level[i] = l
        last_to_enter[l] = i
        while(last_to_enter[l] == i and any((k != i and j >= l) for k, j in enumerate(level))):
            pass
        k += 1
    for x in range(n_loops):
        increment()
    level[i] = -1
    # http://www.csee.wvu.edu/~jdm/classes/cs550/notes/tech/mutex/Peterson2.html
    # https://en.wikipedia.org/wiki/Peterson%27s_algorithm
    # https://docs.python.org/3/library/functions.html#any


def race_m(n_loops, n_threads):
    '''
    Executes the incrementing with Mutex
    '''
    exc = global_var + n_threads * n_loops
    global threadcount
    for i in range(n_threads):
        t = threading.Thread(target=mutex_lock, args=(n_loops, threadLock))
        t.start()
        threadcount += 1
    # Wait till all threads are done
    while threadcount > 0:
        pass
    cur = global_var
    return (exc, cur)
    # http://stackoverflow.com/questions/3310049/proper-use-of-mutexes-in-python


def mutex_lock(n_loops, mutex):
    '''
    Helper function for mutex
    Acquires a lock and releases it once it's done incrementing.
    '''
    global threadcount
    mutex.acquire()
    lockcount.append(mutex)
    try:
        for i in range(n_loops):
            increment()
    finally:
        mutex.release()
        threadcount -= 1


def main():
    threads = 40
    loops = 10
    gil = False
    mutex = False
    peterson = False
    setup_string = "import threading\nimport time\nimport __main__"
    try:
        options, args = getopt.getopt(
            sys.argv[1:], 't:l:hgpm', ['threads', 'loops', 'help', 'gil',
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
                threads = int(arg)
        if opt in ('-l', '--loops'):
            if arg is None:
                break
            else:
                loops = int(arg)
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
        result = race_lock(threads, loops)
        time = timeit.timeit("__main__.race_lock({}, {})".format(
            threads, loops), setup=setup_string, number=1)
    elif peterson is True:
        result = race_p(threads, loops)
        time = timeit.timeit(
            "__main__.race_p({}, {})".format(threads, loops),
            setup=setup_string, number=1)
    elif mutex is True:
        result = race_m(threads, loops)
        time = timeit.timeit(
            "__main__.race_m({}, {})".format(threads, loops),
            setup=setup_string, number=1)
    else:
        result = race(threads, loops)
        time = timeit.timeit(
            "__main__.race({}, {})".format(threads, loops),
            setup=setup_string, number=1)

    # Print results
    print("Threads: {}\tLoops: {}".format(threads, loops))
    print("After {} modifications, global_var should have become {}".format(
        loops * threads, result[0]))
    print("After {} modifications, global_var is {}".format(
        loops * threads, result[1]))
    print("Execution time: {}".format(time))
if __name__ == "__main__":
    main()

# https://www.tutorialspoint.com/python/python_multithreading.htm
