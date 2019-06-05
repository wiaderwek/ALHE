CC = g++
OBJS = graph.o

ALHE: $(OBJS)
	$(CC) -std=c++11 -o ALHE $(OBJS) ALHE.cpp -O2

graph.o: Graph.cpp Graph.h
	$(CC) -c -std=c++11 Graph.cpp -o graph.o -O2

Parser: miniParser.o
	$(CC) -std=c++11 -o Parser miniParser.o parsermain.cpp -O2

miniParser.o: miniParser.cpp miniParser.hpp
	$(CC) -c -std=c++11 miniParser.cpp -o miniParser.o -O2

.PHONY = clean
clean:
	rm ALHE $(OBJS)
