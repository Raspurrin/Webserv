# webserv
## Error Class
### The main purpose of this class is:
#### Catching thrown errors
- Inherits from **std::exception**.
- On construction gets and `_errorCode` and a `_description`.
- Finds in the `errorTypes` map a match and gets the full HTTP error.
- Fills the `_status` struct with the corresponding data.

#### Returning full HTTP status code error

-Returns the filled `_status` struct.

