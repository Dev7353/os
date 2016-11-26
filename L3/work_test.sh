#!/bin/bash
make clean
make work
for((i=1; i <= 10; i++))
{
    sec=$(shuf -i 0-9 -n 1)	
    msec=$(shuf -i 0-999 -n 1)
    echo "./work $sec $msec"
    time ./work $sec $msec >> rand_t.dat
}
exit 0
