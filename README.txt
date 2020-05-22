NAME : VIRAJ SAVALIYA

The sample code provided is for running Unix stream and Unix datagram sockets.
I have tried to make a single file from which we can create any type of Unix socket required.

In both the applications I am taking in command line arguments to create stream or datagram socket. 
Depending on your choice you can use the application programs provided to create either of the sockets for server as well as client. 
Both the sockets are an example of a very basic socket implementation.

To make the application programs type 'make'. This will create executable binaries for the programs.

To run the unix_server use './unix_server <socket-type> <pathname>'
	Here the scoket type can be stream, dgram or seqpacket.
	The pathname is only used for stream sockets

To run the unix_client use './unix_client <socket-type> <pathname/message>'
	Here the scoket type can be stream, dgram or seqpacket.
	The message is used for datagram sockets and teh pathname is used for stream sockets.
	
The detailed explanantion of the sample code and sample outputs are provided in the report in Chapter 7.
