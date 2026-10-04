#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFF_SIZE   5
#define NP          3
#define NC          3
#define NITERS      4

typedef struct {
    int buf[BUFF_SIZE];
    int in;
    int out;
    sem_t full;
    sem_t empty;
    sem_t mutex;
} sbuf_t;

sbuf_t shared;

void *Producer(void *arg);
void *Consumer(void *arg);

int main() {
    return 0;
}
