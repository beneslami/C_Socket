The typical TCP client's communication involves four basic steps:
1- Create a TCP socket using socket().
2- Establish a connection to the server using connect().
3- Communicate using send() and recv().
4- Close the connection with close().

The socket is for IPv4 (AF_INET) using the stream-based protocol(SOCK_STREAM) called TCP(IPPROTO_TCP). The function socket() returns and integer-valueed descriptor  or "handle" for the socket if successful. If socket fails, it returns -1.

The sock-addr_in structure is defined to be a container for the server ip address and port. I called memset() to ensure that any parts of the structure that I do not explicitly set contain zero.

The function inet_pton() converts the string representation of the server's internet address into a 32-bit binary representation. htons() is for converting string-based port number to binary format (host to network short).

The connect() function establishes a connection between the given socket and the one identified by the address and port in the sockaddr_in structure. Because the sockets API is generic, the pointer to the sockaddr_in address structure needs to be cast to the generic type sockaddr, and the actual size of the address data structure must be supplied.

send() returns the number of bytes sent if successful and -1 otherwise.
-------------------------------------------------------------------------------
There are four general steps for basic TCP server communication:
1- Create a TCP socket using socket().
2- Assign a port number to the socket with bind().
3- Tell the system to allow connections to be made to that port, using listen().
4- Repeatedly do the following:
  1) Call accept() to get a new socket for each client connection.
  2) Communicate with the client via that new socket using send() and recv().
  3) Close the client connection using close().

On the server, we need to associate our server socket with and address and port number so that client connections get to the right place. Since we are writing for IPv4, we use a sockaddr_in structure for this. Before setting both address and port number in the sockaddr_in, we convert each to network byte order using htonl() and htons().
The listen() call tells the TCP implementation to allow incoming connections from clients. Before the call to listen(), any incoming connection requests to the socket's address would be silently rejected.
Instead of sending and receiving on the socket, the server application calls accept(), which blocks until an incoming connection is made to the listening socket's port number. At that point, accept() returns a descriptor for a new socket, which is already connected to the initiating remote socket.
The function inet_ntop() takes the binary representation of the client's address and converts it to a dotted-quad string.



Notice: A key principle for coding network applications using sockets is Defensive Programming: your code must not make assumptions about anything received over the network.
