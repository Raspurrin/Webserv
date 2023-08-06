# webserv
## Request Class
### The main purpose of this class is:
- Buffered reading from the socket using `recv`.

- Parse the read request in three main steps:

    - Parsing the start line.
    - Parsing the header fields.
    - Parsing the body/payload if existing.

_While parsing it will catch HTTP errors with the checkers_

    `checkStartLine()`   

If an error is thrown it is catched and saved in a variable of the response instance `_response._requestParsingError`. It will stop parsing and set the variable `isRead` to true in order to be able to meet the condition in `ServerManager` to get the response._

#### Utilites
- `URLDecode()` decodes URI from path.
- `doesKeyExist()` checks for given key in the map, if not found throws the error passed as parameter.
- `checkBoundary()` extracts boundary from content-type and looks for it in the payload.

