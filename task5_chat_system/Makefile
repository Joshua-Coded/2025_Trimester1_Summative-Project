# Makefile for Chat System

CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
LDFLAGS = -pthread

all: server client

server: server.c common.h
	$(CC) $(CFLAGS) -o server server.c $(LDFLAGS)

client: client.c common.h
	$(CC) $(CFLAGS) -o client client.c $(LDFLAGS)

clean:
	rm -f server client *.o

# Run the server
run_server: server
	./server

# Run a client (provide server IP as argument)
# Usage: make run_client IP=127.0.0.1
run_client: client
	./client $(IP)

# Run multiple clients for testing (requires tmux)
run_multiple_clients: client
	tmux new-session -d -s chat_clients './client 127.0.0.1'
	tmux split-window -h './client 127.0.0.1'
	tmux split-window -v './client 127.0.0.1'
	tmux select-pane -t 0
	tmux split-window -v './client 127.0.0.1'
	tmux attach-session -t chat_clients

.PHONY: all clean run_server run_client run_multiple_clients
