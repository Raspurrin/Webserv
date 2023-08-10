#!/usr/bin/python3
import sys

print("Content-Type: text/html")
print("\r\n\r")

for line in sys.stdin:
    if 'Exit' == line.rstrip():
        break
    print(line)