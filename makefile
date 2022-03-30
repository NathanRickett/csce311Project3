text-client:
	g++ text-client.cpp -o t -std=c++17 -Wall -g


text-server:
	g++ text-server.cpp -o s -std=c++17 -Wall -g

all:
	g++ text-client.cpp -o t -std=c++17 -Wall -g
	g++ text-server.cpp -o s -std=c++17 -Wall -g

clean:
	rm t
	rm s
