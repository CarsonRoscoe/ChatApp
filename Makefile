CC=gcc -pthread

all: server

server: server.o
	$(CC) -o server server.o

clean:
	rm -f *.o core.* server

server.o: server.c server.h shared.h
	$(CC) -c server.c
