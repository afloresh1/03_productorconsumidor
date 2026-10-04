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



void *Producer(void *arg)
{
    int i, item, index;
    index = (int)(long)arg;

    for (i = 0; i < NITERS; i++) {
        item = i;	

        /* Esperar si no hay ranuras vacías */
        sem_wait(&shared.empty);
        /* Proteger la sección crítica */
        sem_wait(&shared.mutex);

        shared.buf[shared.in] = item;
        shared.in = (shared.in + 1) % BUFF_SIZE;
        printf("[P%d] Produciendo %d ...\n", index, item); 
        fflush(stdout);

        /* Liberar la sección crítica */
        sem_post(&shared.mutex);
        /* Incrementar el contador de ítems llenos */
        sem_post(&shared.full);

        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}
