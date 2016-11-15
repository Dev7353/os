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
    # for i in range(0, len(numbers)):
    #     sqrt2(int(numbers[i]), True)
    # r1, w1 = os.pipe()
    # r2, w2 = os.pipe()
    # os.close(w1)
    # os.close(r2)
    # r = os.fdopen(r1, 'r')
    # test = r.read()
    # print("Input: " + test)
    # bla = r.read()
    # print("Bla: " + bla)
    # print(sys.argv[1:])
    r, w = map(int, sys.argv[1:])
    test = os.fdopen(r, 'r')
    bla = os.read(r, 14)
    print(bla)
    test.close()
    # r.close()
    # sqrt2(r, True)
    sys.exit(0)

if __name__ == "__main__":
    main()
