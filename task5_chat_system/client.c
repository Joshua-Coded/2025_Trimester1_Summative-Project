
/*
 * client.c - Client for chat system
 * 
 * This client connects to the central server, authenticates,
 * and allows the user to send messages to other clients.
 */

#include "common.h"
#include <stdarg.h>  /* For va_list, va_start, va_end */

/* Global variables */
int sock = -1;
char username[USERNAME_SIZE];
bool running = true;
bool authenticated = false;
pthread_t recv_thread;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Function prototypes */
void *receive_messages(void *arg);
void handle_user_input();
void send_message(int type, const char *receiver, const char *content);
void authenticate();
void request_client_list();
void chat_with_user();
void cleanup();
void safe_print(const char *format, ...);

/* Signal handler for graceful exit */
void handle_signal(int sig) {
    (void)sig;  // Mark parameter as used
    safe_print("\nDisconnecting from server...\n");
    running = false;
    cleanup();
    exit(0);
}

/* Thread-safe printing function */
void safe_print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    pthread_mutex_lock(&print_mutex);
    vprintf(format, args);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);
    
    va_end(args);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    
    /* Set up signal handlers */
    signal(SIGINT, handle_signal);
    
    /* Check arguments */
    if (argc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    /* Configure server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    /* Convert IP address */
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
    
    /* Connect to server */
    printf("Connecting to server at %s:%d...\n", argv[1], PORT);
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Connected to server. Waiting for welcome message...\n");
    
    /* Start receiver thread */
    if (pthread_create(&recv_thread, NULL, receive_messages, NULL) != 0) {
        perror("Failed to create receiver thread");
        cleanup();
        exit(EXIT_FAILURE);
    }
    
    /* Authenticate first */
    authenticate();
    
    /* Then handle user input */
    handle_user_input();
    
    /* Cleanup and exit */
    cleanup();
    return 0;
}

/* Receive messages from server */
void *receive_messages(void *arg) {
    (void)arg;  // Mark parameter as used
    char buffer[BUFFER_SIZE];
    Message message;
    
    /* Receive welcome message */
    ssize_t bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        safe_print("%s", buffer);
    }
    
    /* Wait for authentication to complete before starting main receive loop */
    while (!authenticated && running) {
        sleep(1);
    }
    
    /* Main receive loop */
    while (running) {
        memset(&message, 0, sizeof(Message));  // Clear message structure
        bytes_received = recv(sock, &message, sizeof(Message), 0);
        
        if (bytes_received <= 0) {
            /* Server disconnected */
            safe_print("\nServer disconnected.\n");
            running = false;
            break;
        }
        
        /* Validate message type before processing */
        if (message.type < MSG_AUTH || message.type > MSG_EXIT) {
            // Skip invalid messages
            continue;
        }
        
        /* Process message based on type */
        switch (message.type) {
            case MSG_LIST:
                /* Client list */
                safe_print("\n%s\n", message.content);
                break;
                
            case MSG_CHAT:
                /* Chat message */
                safe_print("\nMessage from %s: %s\n", message.sender, message.content);
                break;
                
            default:
                /* Unknown message type */
                safe_print("\nReceived unknown message type: %d\n", message.type);
                break;
        }
        safe_print("Enter command (list, chat, exit): ");
    }
    
    return NULL;
}

/* Authenticate with server */
void authenticate() {
    printf("Enter your username: ");
    if (fgets(username, USERNAME_SIZE - 1, stdin) == NULL) {
        printf("Error reading username\n");
        return;
    }
    
    /* Remove newline character */
    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }
    
    /* Check if username is empty */
    if (strlen(username) == 0) {
        printf("Username cannot be empty. Please try again.\n");
        authenticate();
        return;
    }
    
    /* Send authentication message */
    send_message(MSG_AUTH, "", "");
    
    printf("Authentication sent. Waiting for confirmation...\n");
    
    /* Wait for response (handled by receive_messages thread) */
    sleep(1);
    
    /* Set authenticated flag */
    authenticated = true;
    
    printf("\nYou are now logged in as '%s'.\n", username);
    printf("Available commands:\n");
    printf("  list - Show online users\n");
    printf("  chat - Send a message to another user\n");
    printf("  exit - Disconnect from server\n\n");
}

/* Handle user input */
void handle_user_input() {
    char command[BUFFER_SIZE];
    
    safe_print("Enter command (list, chat, exit): ");
    while (running && fgets(command, BUFFER_SIZE - 1, stdin) != NULL) {
        /* Remove newline character */
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
        
        /* Process command */
        if (strcmp(command, "list") == 0) {
            request_client_list();
        } else if (strcmp(command, "chat") == 0) {
            chat_with_user();
        } else if (strcmp(command, "exit") == 0) {
            send_message(MSG_EXIT, "", "");
            running = false;
            break;
        } else {
            safe_print("Unknown command. Available commands: list, chat, exit\n");
        }
        
        safe_print("Enter command (list, chat, exit): ");
    }
}

/* Request client list from server */
void request_client_list() {
    send_message(MSG_LIST, "", "");
}

/* Chat with another user */
void chat_with_user() {
    char receiver[USERNAME_SIZE];
    char message_content[BUFFER_SIZE];
    
    /* Get recipient username */
    safe_print("Enter username to chat with: ");
    if (fgets(receiver, USERNAME_SIZE - 1, stdin) == NULL) {
        safe_print("Error reading username\n");
        return;
    }
    
    /* Remove newline character */
    size_t len = strlen(receiver);
    if (len > 0 && receiver[len - 1] == '\n') {
        receiver[len - 1] = '\0';
    }
    
    /* Check if trying to message self */
    if (strcmp(receiver, username) == 0) {
        safe_print("You cannot send a message to yourself.\n");
        return;
    }
    
    /* Get message content */
    safe_print("Enter your message: ");
    if (fgets(message_content, BUFFER_SIZE - 1, stdin) == NULL) {
        safe_print("Error reading message\n");
        return;
    }
    
    /* Remove newline character */
    len = strlen(message_content);
    if (len > 0 && message_content[len - 1] == '\n') {
        message_content[len - 1] = '\0';
    }
    
    /* Send message */
    send_message(MSG_CHAT, receiver, message_content);
    safe_print("Message sent to %s.\n", receiver);
}

/* Send message to server */
void send_message(int type, const char *receiver, const char *content) {
    Message message;
    memset(&message, 0, sizeof(Message));
    message.type = type;
    strcpy(message.sender, username);
    strncpy(message.receiver, receiver, USERNAME_SIZE);
    strncpy(message.content, content, BUFFER_SIZE);
    
    send(sock, &message, sizeof(Message), 0);
}

/* Clean up resources */
void cleanup() {
    if (sock != -1) {
        close(sock);
        sock = -1;
    }
    
    /* Wait for receiver thread to finish */
    if (recv_thread) {
        pthread_cancel(recv_thread);
        pthread_join(recv_thread, NULL);
    }
    
    pthread_mutex_destroy(&print_mutex);
}
