CC = gcc
CFLAGS = -std=c99 -Wall

SERVER = server
CLIENT = client

build: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER).c

$(CLIENT):$(CLIENT).c

clean:
	rm -f $(SERVER) $(CLIENT)
