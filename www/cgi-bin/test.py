#!/usr/bin/python3
import time;
import cgi, cgitb;
import os

form = cgi.FieldStorage()
first_name = form.getvalue("first_name")
last_name = form.getvalue("last_name")
print("Content-Type: text/plain")
hw = "First Name: {fn}, Last Name: {ln}".format(fn = first_name, ln = last_name)

print("\r\n\r")
print(hw)