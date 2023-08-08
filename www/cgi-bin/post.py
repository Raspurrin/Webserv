#!/usr/bin/python3
import time;
import cgi, cgitb;
import os
import sys

form = cgi.FieldStorage()


print("Content-Type: text/html")

print("\r\n\r")

# print(form["myFile"].filename)

if "myFile" in form:
    f = open("in_" + form["myFile"].filename, "wb")
    f.write(form["myFile"].value)

print("Success")