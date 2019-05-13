
CC = g++      # compiler to use
OBJS = ALHE.o graph.o

ALHE: $(OBJS)
				$(CC) -std=c++11 -o ALHE $(OBJS)
graph.o: Graph.cpp Graph.h
	$(CC) -c -std=c++11 Graph.cpp -o graph.o

.PHONY = clean
clean:
				rm ALHE $(OBJS)
