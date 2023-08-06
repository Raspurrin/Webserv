# webserv
## Request Class
### The main purpose of this class is:
- Buffered reading from the socket using `recv`.

- Parse the read request in three main steps:
    1. Parsing the start line.
    2. Parsing the header fields.
    3. Parsing the body/payload if existing.

- While parsing it will catch HTTP errors with the following methods:
    1. `checkStartLine()`
    2. `checkHeaderFields()`

_If an error is catched it will be saved in a variable and it will stop parsing and set the variable `isRead` to true in order to be able to meet the condition in `ServerManager` to get the response._

#### Utilites
- `URLDecode()` decodes URI from path.
- `doesKeyExist()` checks for given key in the map, if not found throws the error passed as parameter.
- `checkBoundary()` extracts boundary from content-type and looks for it in the payload.

