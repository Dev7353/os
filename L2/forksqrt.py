#! usr/bin/env python3
import configparser
import os
import sys

config = configparser.ConfigParser()
config.read("forksqrt.cfg")

start = 0
loops = 0
tolerance = 0.0
numbers = 0


def sqrt2(value, debug):
    xn = start
    # Assert input is a number and bigger than 0
    assert isinstance(value, int), 'Input must be a number!'
    assert value > 0, 'Input must be bigger than 0!'

    if(debug is True):
        print("Testing with var = {:.14f}".format(value))
    for i in range(0, loops):
        s = 1/2 * (xn + value/xn)
        if (debug is True):
            print("Before iteration {}".format(i) + ", s = {:.14f}".format(xn))
        if abs(xn - s) <= tolerance:
            break
        xn = s
    print("After {} iterations, s = {:.14f}".format(i+1, xn))
    return xn


def main():
    r1, w1 = map(int, sys.argv[1:])
    r2, w2 = os.pipe()
    test = os.fdopen(r1, 'r')
    os.close(r2)
    bla = os.read(r1, 1000)
    args = bla.decode("utf-8")
    print("DEBUG ", args)
    args = args.split('|')
    print("CLEAN " + str(args))

    global start
    start = int(args[0])
    global loops
    loops = int(args[1])
    global tolerance
    tolerance = float(args[2])
    numbers = args[3].split(",")
    results = []

    for num in numbers:
        puffer = int(num)
        results.append(sqrt2(puffer, True))
    test.close()
    w2 = os.fdopen(w1, 'w')
    w2.write(str(results))
    w2.close()
    sys.exit(0)

if __name__ == "__main__":
    main()
