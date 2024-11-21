#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>

// Define a custom scheduling policy identifier
#define SCHED_LOW_IDLE 7

// Global variables
char *global_buffer;      // Pointer to the global buffer
char *buffer_ptr;         // Pointer to the current position in the buffer
pthread_mutex_t mutex;    // Mutex for synchronizing access to the buffer
pthread_barrier_t barrier; // Barrier to synchronize the start of threads
int buffer_size;          // Size of the global buffer
int *thread_counts;       // Array to count how many times each thread is scheduled

// Thread function
void *thread_function(void *arg) {
    char character = *(char *)arg; // Character assigned to this thread

    // Wait at the barrier until all threads are ready to start
    pthread_barrier_wait(&barrier);

    // Loop to write the character to the global buffer
    while (1) {
        pthread_mutex_lock(&mutex); // Lock the mutex for access to the buffer
        if (buffer_ptr - global_buffer >= buffer_size) {
            pthread_mutex_unlock(&mutex); // Unlock the mutex if buffer is full
            break; // Exit the loop if the buffer is full
        }
        *buffer_ptr = character; // Write the character to the buffer
        buffer_ptr++; // Move the buffer pointer to the next position
        thread_counts[character - 'A']++; // Increment the count for this thread
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
    return NULL;
}

// Function to print the buffer with post-processing
void print_buffer_with_post_processing() {
    int counts[26] = {0}; // Array to count occurrences of each character
    int new_counts[26] = {0}; // Array to count occurrences of each character after post-processing
    // Count occurrences of each character in the buffer
    for (int i = 0; i < buffer_size; i++) {
        counts[global_buffer[i] - 'A']++;
    }

    // Print unique characters in the buffer
    printf("Unique characters in buffer:\n");
    for (int i = 0; i < 26; i++) {
        if (counts[i] > 0) {
            printf("%c", 'A' + i);
            new_counts[i]++;
        }
    }
    printf("\n");

    // Print counts of each character
    for (int i = 0; i < 26; i++) {
        if (counts[i] > 0) {
            printf("%c = %d\n", 'A' + i, new_counts[i]);
        }
    }
}

// Function to print the scheduling policy and its priority range
void print_sched(int policy)
{
    int priority_min, priority_max;

    // Determine the scheduling policy and print its name
    switch(policy){
        case SCHED_LOW_IDLE:
            printf("SCHED_LOW_IDLE");
            break;
        case SCHED_FIFO:
            printf("SCHED_FIFO");
            break;
        case SCHED_RR:
            printf("SCHED_RR");
            break;
        case SCHED_IDLE:
            printf("SCHED_IDLE");
            break;
        default:
            printf("unknown\n");
    }

    // Get and print the minimum and maximum priority for the given policy
    priority_min = sched_get_priority_min(policy);
    priority_max = sched_get_priority_max(policy);
    printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
}

// Function to set the scheduling policy and priority of a thread
int setpriority(pthread_t *thr, int newpolicy, int newpriority)
{
    int policy, ret;
    struct sched_param param;

    // Check if the new priority is within the valid range for the new policy
    if (newpriority > sched_get_priority_max(newpolicy) || newpriority < sched_get_priority_min(newpolicy)){
        printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));
        return -1;
    }

    // Get and print the current scheduling policy and priority of the thread
    pthread_getschedparam(*thr, &policy, &param);
    printf("current: ");
    print_sched(policy);

    // Set the new scheduling policy and priority
    param.sched_priority = newpriority;
    ret = pthread_setschedparam(*thr, newpolicy, &param);
    if (ret != 0)
        perror("perror(): ");

    // Get and print the new scheduling policy and priority of the thread
    pthread_getschedparam(*thr, &policy, &param);
    printf("new: ");
    print_sched(policy);

    return 0;
}

int main(int argc, char **argv) {
    // Check if the correct number of arguments is provided
    if (argc != 4) {
        printf("Usage: %s <buffer_size> <num_threads> <policy>\n", argv[0]);
        return 1;
    }

    // Parse command-line arguments
    buffer_size = atoi(argv[1]); // Convert buffer size to integer
    int num_threads = atoi(argv[2]); // Convert number of threads to integer
    int policy; // Variable to store the scheduling policy
    int priority = 0; //default priority

    // Determine the scheduling policy based on the input argument
    if (strcmp(argv[3], "SCHED_LOW_IDLE") == 0) {
        policy = SCHED_LOW_IDLE;
    } else if (strcmp(argv[3], "SCHED_IDLE") == 0) {
        policy = SCHED_IDLE;
    } else if (strcmp(argv[3], "SCHED_FIFO") == 0) {
        policy = SCHED_FIFO;
        priority = 1; // Set priority to 1 for SCHED_FIFO
    } else if (strcmp(argv[3], "SCHED_RR") == 0) {
        policy = SCHED_RR;
        priority = 1; // Set priority to 1 for SCHED_RR
    } else {
        printf("Invalid scheduling policy\n");
        return 1;
    }

    // Allocate memory for the global buffer and thread counts
    global_buffer = (char *)malloc(buffer_size * sizeof(char));
    buffer_ptr = global_buffer; // Initialize buffer pointer
    thread_counts = (int *)calloc(num_threads, sizeof(int)); // Initialize thread counts

    pthread_t threads[num_threads]; // Array to store thread identifiers
    char thread_chars[num_threads]; // Array to store characters for each thread

    // Initialize the mutex and barrier
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, num_threads+1); // +1 for the main thread

    // Create and start the threads
    for (int i = 0; i < num_threads; i++) {
        thread_chars[i] = 'A' + i; // Assign a character to each thread
        pthread_create(&threads[i], NULL, thread_function, &thread_chars[i]);
        setpriority(&threads[i], policy, priority); // Set the scheduling policy and priority
    }

    pthread_barrier_wait(&barrier); // Wait for all threads to start

    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex and barrier
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    // Print the buffer without post-processing
    printf("Buffer without post-processing:\n");
    for (int i = 0; i < buffer_size; i++) {
        printf("%c", global_buffer[i]);
    }
    printf("\n\n");

    // Print the buffer with post-processing
    printf("Buffer with post-processing:\n");
    print_buffer_with_post_processing();

    // Print the scheduling count for each thread
    printf("\nThread writings counts:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("%c: %d \n", 'A' + i, thread_counts[i]);
    }

    // Free allocated memory
    free(global_buffer);
    free(thread_counts);

    return 0;
}
