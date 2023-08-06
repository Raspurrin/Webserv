# Webserv
## HTTP server in C++ 98

[Installation instructions](docs/install.md)  
[Technical Documentation](docs/documentation.md)  
### General overview
- **Implemented methods:**
    * _GET:_ Fetch an URI.
    * _POST:_ Send form data to a URI and get a response back.
    * _DELETE:_ Requests that the target resource delete its state.
#### Main system calls
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
- Clients must be able to upload files
- server must be able to listen to multiple ports
- default error pages if none are provided
- accurate HTTP response status codes  
https://en.wikipedia.org/wiki/List_of_HTTP_status_codes  
- Set a default file to answer if the request is a directory.
- Execute CGI based on certain file extension (for example .php).
- Relevant RFCs  
RFC 7230, RFC 7231, RFC 7232, RFC 7233, RFC 7234, RFC 7235  
![image](https://user-images.githubusercontent.com/71138634/231741862-8518d519-c24b-4267-9444-889be657f609.png)
