# webserv
## Response Class
### The main purpose of this class is:
#### Processing HTTP request
- Check for errors thrown during parsing the request.
- Identify the method request.
- Perfom the action the method requires.

#### Building a response

- Reading a file in binary and setting it as response **Body**.
- Getting the MIME type and setting it as **Content-Type**.
- Assembling the response from the response map.

#### HTTP Errors

- When an error is catched it will build the error.
    1. Check if the config file set an error page for the error.
    2. If not, generate an HTML from the template. 
- Generates the HTML from a struct `status` replacing values to match the error.

#### Utilites
- `listDir()` creates a string in html format with a list of files in a directory.
- `setMimes(StringStringMap&)` fills a map with different MIME types.
- `tryChdir(const char*)` tries to execute `chdir` if unsuccesfull, throws an error.

