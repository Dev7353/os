#! usr/bin/env python3
import configparser
import os
import sys

config = configparser.ConfigParser()
config.read("forksqrt.cfg")

start = config.getint("sqrt2", "start")
loops = config.getint("sqrt2", "loops")
tolerance = config.getfloat("sqrt2", "tolerance")
numbers = config.get("sqrt2", "numbers").split(',')


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
    return s


def main():
    r, w = map(int, sys.argv[1:])
    test = os.fdopen(r, 'r')
    bla = os.read(r, 17)
    
    args = bla.decode("utf-8")
    print("DEBUG ", args)
    args = args.split('|')
    print("CLEAN " + str(args))
    
    start = int(args[0])
    loops = int(args[1])
    tolerance = float(args[2])
    numbers = args[3].split(",")
    
    for num in numbers:
		num = int(num)
    
    test.close()
    # r.close()
    for num in numbers:
		sqrt2(num, True)
		
    sys.exit(0)

if __name__ == "__main__":
    main()
