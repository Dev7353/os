#! usr/bin/env python3
import configparser

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
    for i in range(0, len(numbers)):
        sqrt2(int(numbers[i]), True)

if __name__ == "__main__":
    main()
