#!/bin/bash
make clean
make work
for((i=1; i <= 10; i++))
{
    sec=$(shuf -i 1-9 -n 1)	
    msec=$(shuf -i 0-999 -n 1)
    echo "$sec$msec `./work $sec $msec | grep -o [0-9]*`" >> rand_t.dat
}
