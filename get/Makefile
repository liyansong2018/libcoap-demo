LIBRARIES = -lcoap-2
INCLUDE = -I /usr/local/include/coap2/
CXXFLAGS = -O2 -D_GNU_SOURCE -DWITH_POSIX

all: server client
	
server:server.c
	gcc server.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  server
	
client:client.c
	gcc client.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  client