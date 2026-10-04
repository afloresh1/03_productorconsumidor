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

int main()
{
    pthread_t idP[NP], idC[NC];
    long index;

    /* Inicializar semáforos */
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    sem_init(&shared.mutex, 0, 1);

    shared.in = 0;
    shared.out = 0;

    /* Crear hilos productores */
    for (index = 0; index < NP; index++) {  
       pthread_create(&idP[index], NULL, Producer, (void*)index);
    }

    /* Crear hilos consumidores */
    for (index = 0; index < NC; index++) {
       pthread_create(&idC[index], NULL, Consumer, (void*)index);
    }

    pthread_exit(NULL);
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
void *Consumer(void *arg)
{
    int i, item, index;
    index = (int)(long)arg;

    for (i = 0; i < NITERS; i++) {
        /* Esperar si no hay elementos llenos */
        sem_wait(&shared.full);
        /* Proteger la sección crítica */
        sem_wait(&shared.mutex);

        item = shared.buf[shared.out];
        shared.out = (shared.out + 1) % BUFF_SIZE;
        printf("-----> [C%d] consumido %d\n", index, item); 
        fflush(stdout);

        /* Liberar la sección crítica */
        sem_post(&shared.mutex);
        /* Incrementar el contador de ranuras vacías */
        sem_post(&shared.empty);

        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

