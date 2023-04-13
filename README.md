# Webserv
## HTTP server in C++ 98

- GET, POST, and DELETE methods.
    1. GET: Fetch an URL  
    2. POST: Send form data to a URL and get a response back  
    3. DELETE: The DELETE method requests that the target resource delete its state.  
https://en.wikipedia.org/wiki/HTTP#Request_methods  
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
