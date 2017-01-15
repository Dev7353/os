#!/usr/bin/python

import sys
import matplotlib.pyplot as plt

def main():
    cats = []
    dogs = []
    mice = []

    animal = 0
    f = open(sys.argv[1], 'r')

    for line in f.readlines():
        if line == "cats\n":
            animal = 0
            continue
        elif line == "dogs\n":
            animal = 1
            continue
        elif line == "mice\n":
            animal = 2
            continue

        if animal == 0:
            c = line.replace("\n", "").split(",")
            cats.append((float(c[0]), float(c[1])))
        elif animal == 1:
            d = line.replace("\n", "").split(",")
            dogs.append((float(d[0]), float(d[1])))
        else:
            m = line.replace("\n", "").split(",")
            mice.append((float(m[0]), float(m[1])))


    plt.xlabel("Group waiting time [sec]")
    plt.ylabel("Thread waiting time [sec]")

    plt.scatter(*zip(*mice))
    file = sys.argv[1] + "_mice.eps"
    plt.savefig(file)

    plt.scatter(*zip(*dogs))
    file = sys.argv[1] + "_dogs.eps"
    plt.savefig(file)

    plt.scatter(*zip(*cats))
    file = sys.argv[1] + "_cats.eps"
    plt.savefig(file)

if __name__ == "__main__":
    main()
