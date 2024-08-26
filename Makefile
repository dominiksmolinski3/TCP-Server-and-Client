all: server client

server: server.cpp
	g++ server.cpp -o server -lpthread

client: client.cpp
	g++ client.cpp -o client

clean:   
	rm -f server client