#!/usr/bin/python
print("HTTP/1.1 200 OK\nContent-Type: text/html")
hw = "Hello World"
print("Content-Length: %i\n" % len(hw))
print(hw)