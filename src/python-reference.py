#!/bin/python3

with open('expected.txt', 'w') as f:
    i = 2
    k = i ** 9000 - 1 + 1000
    f.write(str(k) + "\n")