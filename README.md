# Webserv
## HTTP server in C++ 98

[Installation instructions](docs/install.md)  
[Technical Documentation](docs/documentation.md)  
[Subject](docs/webserv.pdf)  
### HTTP overview
- **Implemented methods:**
    * _GET:_ Fetch an URI.
    * _POST:_ Send form data to a URI and get a response back.
    * _DELETE:_ Requests that the target resource delete its state.
- **HTTP Version:**
    * HTTP/1.1
- **HTTP Error Max Values:**  

| Error                           | Max Value      | Method          | Change Value in:                 |
|:--------------------------------|:--------------:|:---------------:|:---------------------------------|
| Request Timeout                 | 2              | all implemented | webserv.hpp                      |
| Content Too Large               | 1GB per chunk  | POST            | config file or Request.class.cpp |
| URI Too Long                    | 255 bytes      | all implemented | Request.class.cpp                |
| Request Header Fields Too Large | 9000 bytes     | all implemented | Request.class.cpp                |
| Unsupported Media Type          | multipart/form | POST            | Not implemented                  |

### Main system calls
- **Asynchronous I/O:**
    * [poll()](https://www.man7.org/linux/man-pages/man2/poll.2.html)
    * [recv()](https://www.man7.org/linux/man-pages/man2/recv.2.html)
- **Sockets:**
    * [socket()](https://www.man7.org/linux/man-pages/man2/socket.2.html)
    * [fcntl()](https://www.man7.org/linux/man-pages/man2/fcntl.2.html)
    * [setsockopt()](https://www.man7.org/linux/man-pages/man2/setsockopt.2.html)
    * [bind()](https://www.man7.org/linux/man-pages/man2/bind.2.html)
    * [listen()](https://www.man7.org/linux/man-pages/man2/listen.2.html)
    * [accept()](https://www.man7.org/linux/man-pages/man2/accept.2.html)
    * [send()](https://www.man7.org/linux/man-pages/man2/send.2.html)
    * [close()](https://www.man7.org/linux/man-pages/man2/close.2.html)

- Relevant RFCs  
RFC 7230, RFC 7231, RFC 7232, RFC 7233, RFC 7234, RFC 7235  

![image](https://user-images.githubusercontent.com/71138634/231741862-8518d519-c24b-4267-9444-889be657f609.png)
