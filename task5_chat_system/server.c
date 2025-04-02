/*
 * server.c - Central server for chat system
 * 
 * This server can handle up to 4 clients simultaneously.
 * It manages client authentication and message routing.
 */

#include "common.h"

/* Global variables */
Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Function prototypes */
void *handle_client(void *arg);
void broadcast_client_list();
void send_client_list(int client_index);
void route_message(Message message, int sender_index);
void disconnect_client(int client_index);
int find_client_by_username(const char *username);
int find_available_slot();
void cleanup();

/* Signal handler for graceful exit */
void handle_signal(int sig) {
    (void)sig;  // Mark parameter as used
    printf("\nShutting down server...\n");
    cleanup();
    exit(0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    pthread_t tid;
    socklen_t client_len = sizeof(client_addr);
    
    /* Initialize clients array */
    memset(clients, 0, sizeof(clients));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = -1;
        clients[i].authenticated = false;
        clients[i].connected = false;
    }
    
    /* Set up signal handlers */
    signal(SIGINT, handle_signal);
    
    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    /* Set socket options to reuse address */
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    /* Configure server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    /* Bind socket */
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    /* Listen for connections */
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server started on port %d. Waiting for connections...\n", PORT);
    
    /* Accept connections */
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }
        
        printf("New connection from %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        /* Check if server is full */
        int slot = find_available_slot();
        if (slot == -1) {
            printf("Server is full. Connection rejected.\n");
            char *msg = "Server is full. Try again later.\n";
            send(client_fd, msg, strlen(msg), 0);
            close(client_fd);
            continue;
        }
        
        /* Add client to array */
        pthread_mutex_lock(&clients_mutex);
        clients[slot].socket = client_fd;
        clients[slot].connected = true;
        pthread_mutex_unlock(&clients_mutex);
        
        /* Create thread to handle client */
        if (pthread_create(&tid, NULL, handle_client, (void *)(intptr_t)slot) != 0) {
            perror("pthread_create failed");
            pthread_mutex_lock(&clients_mutex);
            clients[slot].socket = -1;
            clients[slot].connected = false;
            pthread_mutex_unlock(&clients_mutex);
            close(client_fd);
        }
        
        /* Detach thread */
        pthread_detach(tid);
    }
    
    /* Clean up */
    cleanup();
    return 0;
}

/* Handle client connection */
void *handle_client(void *arg) {
    int index = (intptr_t)arg;
    int sock = clients[index].socket;
    Message message;
    
    /* Send welcome message */
    char welcome[BUFFER_SIZE];
    sprintf(welcome, "Welcome to the chat server! Please login with your username.\n");
    send(sock, welcome, strlen(welcome), 0);
    
    while (1) {
        /* Receive message */
        memset(&message, 0, sizeof(Message));  // Clear message structure
        ssize_t bytes_received = recv(sock, &message, sizeof(Message), 0);
        if (bytes_received <= 0) {
            /* Client disconnected */
            disconnect_client(index);
            break;
        }
        
        /* Validate message type */
        if (message.type < MSG_AUTH || message.type > MSG_EXIT) {
            printf("Received invalid message type %d from client %d\n", 
                   message.type, index);
            continue;
        }
        
        /* Process message based on type */
        switch (message.type) {
            case MSG_AUTH:
                /* Authentication request */
                pthread_mutex_lock(&clients_mutex);
                
                /* Check if username is already taken */
                int existing = find_client_by_username(message.sender);
                if (existing != -1 && existing != index) {
                    char *error = "Username already taken. Please choose another one.\n";
                    send(sock, error, strlen(error), 0);
                } else {
                    /* Authenticate client */
                    strncpy(clients[index].username, message.sender, USERNAME_SIZE);
                    clients[index].authenticated = true;
                    
                    /* Confirm authentication */
                    char confirm[BUFFER_SIZE];
                    sprintf(confirm, "Authentication successful. Welcome, %s!\n", message.sender);
                    send(sock, confirm, strlen(confirm), 0);
                    
                    printf("Client authenticated: %s\n", clients[index].username);
                    
                    /* Broadcast updated client list */
                    broadcast_client_list();
                }
                
                pthread_mutex_unlock(&clients_mutex);
                break;
                
            case MSG_LIST:
                /* List request */
                if (clients[index].authenticated) {
                    send_client_list(index);
                }
                break;
                
            case MSG_CHAT:
                /* Chat message */
                if (clients[index].authenticated) {
                    route_message(message, index);
                }
                break;
                
            case MSG_EXIT:
                /* Exit request */
                disconnect_client(index);
                return NULL;
                
            default:
                /* Unknown message type */
                printf("Unknown message type: %d\n", message.type);
                break;
        }
    }
    
    return NULL;
}

/* Find client by username */
int find_client_by_username(const char *username) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].authenticated && 
            clients[i].connected && 
            strcmp(clients[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

/* Find available slot for new client */
int find_available_slot() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].connected) {
            return i;
        }
    }
    return -1;
}

/* Broadcast client list to all authenticated clients */
void broadcast_client_list() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].authenticated && clients[i].connected) {
            send_client_list(i);
        }
    }
}

/* Send client list to specific client */
void send_client_list(int client_index) {
    Message message;
    memset(&message, 0, sizeof(Message));
    message.type = MSG_LIST;
    strcpy(message.sender, "SERVER");
    strcpy(message.receiver, clients[client_index].username);
    
    /* Construct list of online users */
    memset(message.content, 0, BUFFER_SIZE);
    strcpy(message.content, "Online users:\n");
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].authenticated && clients[i].connected) {
            strcat(message.content, "- ");
            strcat(message.content, clients[i].username);
            strcat(message.content, "\n");
        }
    }
    
    /* Send list */
    send(clients[client_index].socket, &message, sizeof(Message), 0);
}

/* Route message to recipient */
void route_message(Message message, int sender_index) {
    /* Verify sender */
    if (strcmp(message.sender, clients[sender_index].username) != 0) {
        printf("Warning: Spoofed message from %s\n", clients[sender_index].username);
        return;
    }
    
    /* Find recipient */
    int recipient_index = find_client_by_username(message.receiver);
    if (recipient_index == -1) {
        /* Recipient not found */
        Message response;
        memset(&response, 0, sizeof(Message));
        response.type = MSG_CHAT;
        strcpy(response.sender, "SERVER");
        strcpy(response.receiver, message.sender);
        sprintf(response.content, "User %s is not online.\n", message.receiver);
        
        send(clients[sender_index].socket, &response, sizeof(Message), 0);
        return;
    }
    
    /* Send message to recipient */
    send(clients[recipient_index].socket, &message, sizeof(Message), 0);
    
    printf("Message routed from %s to %s\n", message.sender, message.receiver);
}

/* Disconnect client */
void disconnect_client(int client_index) {
    pthread_mutex_lock(&clients_mutex);
    
    if (clients[client_index].connected) {
        printf("Client disconnected: %s\n", 
               clients[client_index].authenticated ? clients[client_index].username : "unauthenticated");
        
        close(clients[client_index].socket);
        clients[client_index].socket = -1;
        clients[client_index].authenticated = false;
        clients[client_index].connected = false;
        memset(clients[client_index].username, 0, USERNAME_SIZE);
    }
    
    pthread_mutex_unlock(&clients_mutex);
    
    /* Broadcast updated client list */
    broadcast_client_list();
}

/* Clean up resources */
void cleanup() {
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].connected) {
            close(clients[i].socket);
            clients[i].socket = -1;
            clients[i].authenticated = false;
            clients[i].connected = false;
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
    pthread_mutex_destroy(&clients_mutex);
}
