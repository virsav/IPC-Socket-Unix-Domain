NAME : VIRAJ SAVALIYA


To make the application programs type 'make'. This will create executable binaries for the programs.

To run the unix_server use './unix_server <socket-type> <pathname>'
	Here the scoket type can be stream, dgram or seqpacket.
	The pathname is only used for stream sockets

To run the unix_client use './unix_client <socket-type> <pathname/message>'
	Here the scoket type can be stream, dgram or seqpacket.
	The message is used for datagram sockets and teh pathname is used for stream sockets.
