#! /usr/bin/env python3 -tt

import cyforksqrt
import forksqrt
import timeit
import math


def main():
    value = 5.0
    cysqrtm_result = cyforksqrt.sqrtm(value)
    cysqrtm_time = timeit.timeit(
        "cyforksqrt.sqrtm(" + str(value) + ")", globals=globals())
    print_results("cyforksqrt.sqrtm", cysqrtm_result, cysqrtm_time)

    cysqrt2_result = cyforksqrt.sqrt2(value)
    cysqrt2_time = timeit.timeit(
        "cyforksqrt.sqrt2(" + str(value) + ")", globals=globals())
    print_results("cyforksqrt.sqrt2", cysqrt2_result, cysqrt2_time)

    forksqrt2_result = forksqrt.sqrt2(value, False)
    forksqrt2_time = timeit.timeit(
        "forksqrt.sqrt2(" + str(value) + ", False)", globals=globals())
    print_results("forksqrt.sqrt2", forksqrt2_result, forksqrt2_time)

    mathsqrt_result = math.sqrt(value)
    mathsqrt_time = timeit.timeit(
        "math.sqrt(" + str(value) + ")", globals=globals())
    print_results("math.sqrt", mathsqrt_result, mathsqrt_time)


def print_results(name, result, time):
    print('---{}( 5.0 )---\nResult: {}\nExecution Time: {}ms'
          .format(name, result, time))


if __name__ == "__main__":
    main()
