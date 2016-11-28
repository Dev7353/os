#! usr/bin/env python3
import os
import sys
import decimal

start = 1
loops = 100
tolerance = 1e-14
numbers = 0
decimal.getcontext().prec = 15


def sqrt2(value, debug):
    """
    @param value: Input value
    @param debug: Boolean value if every calculation step should logged
    @return: Returns the calculate root of value
    """
    xn = decimal.Decimal(start)
    # Assert input is a number and bigger than 0
    # assert value.isdigit(), 'Input must be a number!'
    value = float(value)
    # assert value > 0, 'Input must be bigger than 0!'
    if(debug is True):
        print("Testing with var = {:.14f}".format(value))
    for i in range(0, loops):
        s = decimal.Decimal(1/2) * (xn + decimal.Decimal(value)/xn)
        if (debug is True):
            print("Before iteration {}".format(i) + ", s = {:.14f}".format(xn))
        if abs(xn - s) <= tolerance:
            break
        xn = s
    if(debug is True):
        print("After {} iterations, s = {:.14f}\n".format(i+1, xn))
    return float(xn)


def main():
    # Open the two pipes to read and write from and to parent
    r1, w1 = map(int, sys.argv[1:])
    r2, w2 = os.pipe()

    # Read from parent
    fd = os.fdopen(r1, 'r')
    os.close(r2)  # Close unused read side of pipe
    fd_read = os.read(r1, 1000)
    args = fd_read.decode("utf-8")
    args = args.split('|')  # Split the input by seperator '|'

    # Assign global variables and parameters to input
    global start
    start = int(args[0])
    global loops
    loops = int(args[1])
    global tolerance
    tolerance = float(args[2])
    numbers = args[3].split(",")
    if(args[4] == "True"):
        debug = True
    else:
        debug = False

    # Calculate the square roots
    results = []
    for num in numbers:
        results.append(sqrt2(num, debug))
    fd.flush()
    fd.close()

    # Pipe the results back to parents as a string
    w2 = os.fdopen(w1, 'w')
    w2.write(str(results))
    w2.flush()
    w2.close()

    sys.exit(0)

if __name__ == "__main__":
    main()
