import sys
import numpy
import os


def main():
    if len(sys.argv) != 5:
        print(
            "Try the following argument rule: python matr-gen.py row col min max")
        exit(1)

    row = int(sys.argv[1])
    col = int(sys.argv[2])
    minimum = int(sys.argv[3])
    maximum = int(sys.argv[4])

    if row < 0 or col < 0:
        print("Did you see ever a matrix with negativ sizes?")
        exit(1)

    if(row != col):
        print(
            "Matrix multiplication only supports nxn. Anyway i continue with the first number.")

    col = row

    array = numpy.random.random_integers(
        low=minimum, high=maximum, size=(row, col))

    f = "matr.0"
    while os.path.isfile(f):
        number = f[len(f) - 1]
        number = int(number) + 1
        f = f.replace(f[len(f) - 1], str(number))

    numpy.savetxt(f, array, fmt='%.5f')

if __name__ == "__main__":
    main()
