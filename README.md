CMP425 / CMP501 
Lab 1 TCP server / client example - by Andrei Boiko
Built using SFML Sockets

### TCP Server
A simple TCP server that waits for a connection. 
When a connection is made, the server sends "hello" to the client.
It then repeats back anything it receives from the client.
All the calls are blocking -- so this program only handles one connection at a time.

### TCP Client
A simple client that connects to a server and waits for a response. 
The server sends "hello" when the client first connects. 
Text typed is then sent to the server which echoes it back, and the response is printed out.
