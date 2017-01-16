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
        elif line == "---\n":
            break

        if animal == 0:
            c = line.replace("\n", "").split(",")
            cats.append((float(c[0]), float(c[1])))
        elif animal == 1:
            d = line.replace("\n", "").split(",")
            dogs.append((float(d[0]), float(d[1])))
        else:
            m = line.replace("\n", "").split(",")
            mice.append((float(m[0]), float(m[1])))


    plt.ylabel("Thread waiting time [sec]")

    if mice:
        plt.xlabel("Mice * eating times")
        plt.plot(*zip(*mice))
        file = sys.argv[1] + "_mice.eps"
        plt.savefig(file)
        plt.clf()
        plt.cla()
        plt.close()
        
    if dogs:
        plt.xlabel("Dogs * eating times")
        plt.plot(*zip(*dogs))
        file = sys.argv[1] + "_dogs.eps"
        plt.savefig(file)
        plt.clf()
        plt.cla()
        plt.close()
    if cats:
        plt.xlabel("Cats * eating times")
        plt.plot(*zip(*cats))
        file = sys.argv[1] + "_cats.eps"
        plt.savefig(file)
        plt.clf()
        plt.cla()
        plt.close()

if __name__ == "__main__":
    main()
