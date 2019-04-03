CC = gcc
CFLAGS = -std=c99 -Wall

SERVER = server
CLIENT = client

build: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER).o msg_queue.o array_list.o

$(CLIENT):$(CLIENT).o msg_queue.o

$(SERVER).o: $(SERVER).c msg_queue.h msg_format.h array_list.h

$(CLIENT).o: $(CLIENT).c msg_queue.h msg_format.h

clean:
	rm -f $(SERVER) $(CLIENT) $(SERVER).o $(CLIENT).o msg_queue.o array_list.o
