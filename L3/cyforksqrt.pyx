from libc.math cimport sqrt


def sqrtm(value):
    return sqrt(value)


def sqrt2(value):
    """
    @param value: Input value
    @return: Returns the calculate root of value
    """
    cdef float start = 1.0
    cdef int loops = 100
    cdef float tolerance = 1e-14
    cdef float xn = start
    cdef float s

    for i in range(loops):
        s = 0.5 * (xn + value/xn)
        if abs(xn - s) <= tolerance:
            break
        xn = s
    return xn
