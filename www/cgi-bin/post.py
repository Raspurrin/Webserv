#!/usr/bin/python3
import time;
import cgi, cgitb;
import os
import sys

form = cgi.FieldStorage()

if "myFile" in form:
    f = open("www/cgi-bin/" + form["myFile"].filename, "wb")
    f.write(form["myFile"].value)
    print("Status: 201")
else:
    print("Status: 400")

print("Content-Type: text/html")

print("\r\n\r")

# print(form["myFile"].filename)