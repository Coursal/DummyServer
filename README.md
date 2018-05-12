# DummyServer
A (way too) simple concurrent server for (way too) simple things, such as:

* calculating the average of an array of integers (X)
* finding the max and min element of an array of integers (X)
* calculating the product of an array of integers (X) with a float number (r)

### Guide to compile and run:

#### Server:
1. `gcc -pthread -o server server.c`
2. `./server *port_number*`

#### Client:
1. `gcc -pthread -o server server.c`
2. `./client *host_name* *port_number*`
