#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <linux/sched.h>

#define SCHED_LOW_IDLE 7

char *global_buffer;
int buffer_size;
int current_position = 0;
sem_t semaphore;

void *thread_function(void *arg)
{
    char thread_char = *(char *)arg;
    while (1)
    {
        sem_wait(&semaphore);
        if (current_position >= buffer_size)
        {
            sem_post(&semaphore);
            break;
        }
        global_buffer[current_position++] = thread_char;
        sem_post(&semaphore);
        usleep(100); // Simulate some work
    }
    return NULL;
}

void post_process_and_print(int num_threads, char *thread_chars)
{
    printf("Buffer sem pós-processamento:\n%s\n", global_buffer);

    printf("Buffer após pós-processamento:\n");
    char last_char = global_buffer[0];
    printf("%c", last_char);
    for (int i = 1; i < buffer_size; i++)
    {
        if (global_buffer[i] != last_char)
        {
            printf("%c", global_buffer[i]);
            last_char = global_buffer[i];
        }
    }
    printf("\n");

    // Count occurrences
    for (int i = 0; i < num_threads; i++)
    {
        int count = 0;
        for (int j = 0; j < buffer_size; j++)
        {
            if (global_buffer[j] == thread_chars[i])
            {
                count++;
            }
        }
        printf("%c = %d\n", thread_chars[i], count);
    }
}

int set_thread_policy(pthread_t thread, int policy)
{
    struct sched_param param;
    param.sched_priority = 0; // Default priority for non-realtime policies

    if (pthread_setschedparam(thread, policy, &param) != 0)
    {
        perror("Failed to set thread scheduling policy");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <buffer_size> <num_threads> <policy>\n", argv[0]);
        return EXIT_FAILURE;
    }

    buffer_size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int policy;

    if (strcmp(argv[3], "SCHED_LOW_IDLE") == 0)
    {
        policy = SCHED_LOW_IDLE;
    }
    else if (strcmp(argv[3], "SCHED_IDLE") == 0)
    {
        policy = SCHED_IDLE;
    }
    else if (strcmp(argv[3], "SCHED_FIFO") == 0)
    {
        policy = SCHED_FIFO;
    }
    else if (strcmp(argv[3], "SCHED_RR") == 0)
    {
        policy = SCHED_RR;
    }
    else
    {
        fprintf(stderr, "Invalid policy. Use SCHED_LOW_IDLE, SCHED_IDLE, SCHED_FIFO, or SCHED_RR.\n");
        return EXIT_FAILURE;
    }

    global_buffer = malloc(buffer_size);
    if (!global_buffer)
    {
        perror("Failed to allocate buffer");
        return EXIT_FAILURE;
    }
    memset(global_buffer, 0, buffer_size);

    pthread_t threads[num_threads];
    char thread_chars[num_threads];
    sem_init(&semaphore, 0, 1);

    for (int i = 0; i < num_threads; i++)
    {
        thread_chars[i] = 'A' + i;
        pthread_create(&threads[i], NULL, thread_function, &thread_chars[i]);
        set_thread_policy(threads[i], policy);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    post_process_and_print(num_threads, thread_chars);

    free(global_buffer);
    sem_destroy(&semaphore);

    return EXIT_SUCCESS;
}
