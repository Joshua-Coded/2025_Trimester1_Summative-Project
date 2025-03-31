/*
 * producer_consumer.c - Implementation of a producer-consumer model
 *
 * This program simulates a manufacturing assembly line where a producer
 * adds items to a queue and a consumer removes them. It demonstrates
 * thread synchronization using mutexes and condition variables.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

/* Configuration constants */
#define QUEUE_SIZE 10    /* Maximum items in queue */
#define PRODUCER_DELAY 2 /* Seconds between production */
#define CONSUMER_DELAY 3 /* Seconds between consumption */

/* Flag for graceful termination */
volatile int running = 1;

/* Shared queue data structure */
typedef struct
{
    int items[QUEUE_SIZE];    /* Array to store items */
    int count;                /* Current number of items */
    pthread_mutex_t mutex;    /* Mutex for synchronization */
    pthread_cond_t not_full;  /* Condition: queue is not full */
    pthread_cond_t not_empty; /* Condition: queue is not empty */
} queue_t;

/* Global queue */
queue_t queue;

/* SIGINT handler for clean termination */
void handle_sigint(int sig)
{
    printf("\nReceived termination signal. Shutting down...\n");
    running = 0;

    /* Signal both conditions to wake up waiting threads */
    pthread_cond_signal(&queue.not_empty);
    pthread_cond_signal(&queue.not_full);
}

/* Initialize queue and synchronization primitives */
void initialize_queue()
{
    queue.count = 0;
    pthread_mutex_init(&queue.mutex, NULL);
    pthread_cond_init(&queue.not_full, NULL);
    pthread_cond_init(&queue.not_empty, NULL);
}

/* Cleanup queue resources */
void destroy_queue()
{
    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.not_full);
    pthread_cond_destroy(&queue.not_empty);
}

/* Display current queue status */
void display_queue()
{
    printf("[");
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (i < queue.count)
        {
            printf("█");
        }
        else
        {
            printf(" ");
        }

        if (i < QUEUE_SIZE - 1)
        {
            printf("|");
        }
    }
    printf("] (%d/%d items)\n", queue.count, QUEUE_SIZE);
}

/* Producer thread function */
void *producer(void *arg)
{
    int item = 1;

    while (running)
    {
        /* Simulate production time */
        sleep(PRODUCER_DELAY);

        /* Acquire mutex */
        pthread_mutex_lock(&queue.mutex);

        /* Wait if queue is full */
        while (queue.count == QUEUE_SIZE && running)
        {
            printf("Producer: Queue is full, waiting...\n");
            pthread_cond_wait(&queue.not_full, &queue.mutex);
        }

        /* Check if we're shutting down */
        if (!running)
        {
            pthread_mutex_unlock(&queue.mutex);
            break;
        }

        /* Add item to queue */
        queue.items[queue.count] = item;
        queue.count++;

        printf("Producer: Item %d added to the assembly line\n", item);
        display_queue();

        item++; /* Next item number */

        /* Signal that queue is not empty */
        pthread_cond_signal(&queue.not_empty);

        /* Release mutex */
        pthread_mutex_unlock(&queue.mutex);
    }

    printf("Producer: Terminating\n");
    return NULL;
}

/* Consumer thread function */
void *consumer(void *arg)
{
    while (running)
    {
        /* Simulate consumption time */
        sleep(CONSUMER_DELAY);

        /* Acquire mutex */
        pthread_mutex_lock(&queue.mutex);

        /* Wait if queue is empty */
        while (queue.count == 0 && running)
        {
            printf("Consumer: Queue is empty, waiting...\n");
            pthread_cond_wait(&queue.not_empty, &queue.mutex);
        }

        /* Check if we're shutting down */
        if (!running && queue.count == 0)
        {
            pthread_mutex_unlock(&queue.mutex);
            break;
        }

        /* Remove item from queue */
        int item = queue.items[0];

        /* Shift remaining items */
        for (int i = 0; i < queue.count - 1; i++)
        {
            queue.items[i] = queue.items[i + 1];
        }

        queue.count--; /* Decrement counter */

        printf("Consumer: Item %d removed from the assembly line\n", item);
        display_queue();

        /* Signal that queue is not full */
        pthread_cond_signal(&queue.not_full);

        /* Release mutex */
        pthread_mutex_unlock(&queue.mutex);
    }

    printf("Consumer: Terminating\n");
    return NULL;
}

int main()
{
    pthread_t producer_thread, consumer_thread;
    struct sigaction sa;

    /* Set up signal handler for graceful termination */
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    /* Initialize the queue */
    initialize_queue();

    /* Print startup message */
    printf("Starting Producer-Consumer Simulation\n");
    printf("Producer adds items every %d seconds\n", PRODUCER_DELAY);
    printf("Consumer removes items every %d seconds\n", CONSUMER_DELAY);
    printf("Maximum queue size: %d items\n\n", QUEUE_SIZE);
    printf("Press Ctrl+C to terminate\n\n");

    /* Create threads */
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    /* Wait for threads to finish */
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    printf("Simulation ended\n");

    /* Cleanup */
    destroy_queue();

    return 0;
}