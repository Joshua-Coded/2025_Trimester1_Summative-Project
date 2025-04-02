/*
 * common.h - Shared definitions for chat system
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>

#define PORT 8888
#define MAX_CLIENTS 4
#define BUFFER_SIZE 1024
#define USERNAME_SIZE 32

/* Message types */
#define MSG_AUTH 1      // Authentication message
#define MSG_LIST 2      // List online users
#define MSG_CHAT 3      // Chat message
#define MSG_EXIT 4      // Client exit

/* Message structure */
typedef struct {
    int type;                       // Message type
    char sender[USERNAME_SIZE];     // Sender's username
    char receiver[USERNAME_SIZE];   // Receiver's username
    char content[BUFFER_SIZE];      // Message content
} __attribute__((packed)) Message;

/* Client structure */
typedef struct {
    int socket;                     // Client socket
    char username[USERNAME_SIZE];   // Client username
    bool authenticated;             // Authentication status
    bool connected;                 // Connection status
} Client;

#endif /* COMMON_H */
