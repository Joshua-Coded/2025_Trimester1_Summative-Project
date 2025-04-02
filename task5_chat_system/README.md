# Chat System

This is a simple chat system with a central server that manages multiple clients. The server allows clients to authenticate with a username, see other online users, and exchange messages.

## Features

- Central server that manages up to 4 clients
- Client authentication with username
- View list of online clients
- Private messaging between clients
- Real-time bidirectional communication
- Only authenticated clients can communicate

## Requirements

- GCC compiler
- POSIX-compliant system (Linux, macOS)
- pthread library

## Compilation

To compile both the server and client programs:

```bash
make
```

This will create two executables:
- `server` - The central server program
- `client` - The client program

## Usage

### Starting the Server

```bash
./server
```

The server will start and listen for connections on port 8888.

### Starting a Client

```bash
./client <server_ip>
```

Replace `<server_ip>` with the IP address of the server. If running locally, use `127.0.0.1`.

Example:
```bash
./client 127.0.0.1
```

### Client Commands

Once connected, clients can use the following commands:

- `list` - View a list of online users
- `chat` - Send a message to another user
- `exit` - Disconnect from the server

### Running Multiple Clients for Testing

If you have tmux installed, you can start multiple clients at once:

```bash
make run_multiple_clients
```

This will open a tmux session with 4 client windows. To exit tmux, press `Ctrl+b` then `d`.

## System Architecture

### Common Components (common.h)
- Defines shared structures and constants
- Message structure for communication
- Client structure for client management

### Server (server.c)
- Manages client connections
- Handles client authentication
- Routes messages between clients
- Tracks online users
- Handles up to 4 simultaneous connections

### Client (client.c)
- Connects to the central server
- Authenticates with a username
- Sends and receives messages
- Provides a command-line interface

## Protocol

The chat system uses a simple binary protocol for communication, with messages structured as follows:

- Message Type (int)
- Sender Username (string)
- Receiver Username (string)
- Message Content (string)

Message types:
- AUTH (1): Authentication request
- LIST (2): Request online user list
- CHAT (3): Chat message
- EXIT (4): Client disconnection

## Limitations

- Maximum of 4 clients can connect simultaneously
- Usernames are limited to 31 characters
- Messages are limited to 1023 characters
- No encryption or security features
- No persistent storage of messages
