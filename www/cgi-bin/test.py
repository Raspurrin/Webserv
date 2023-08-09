#!/usr/bin/python3
import time;
import cgi, cgitb;
import os

form = cgi.FieldStorage()
first_name = form.getvalue("first_name")
last_name = form.getvalue("last_name")
print("Content-Type: text/html")
hw = "<p>First Name: {fn}, Last Name: {ln}</p>".format(fn = first_name, ln = last_name)
# print(f"Content-Length: {len(hw) + 1}")

print("\r\n\r")
print(hw)
for k, v in os.environ.items():
    print(f'<p>{k}={v}</p>')