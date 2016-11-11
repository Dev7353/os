
def sqrt2(value, debug):
    start = 1
    loops = 100
    tolerance = 1.e-14
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
    sqrt2(21213124324233, True)

if __name__ == "__main__":
    main()
