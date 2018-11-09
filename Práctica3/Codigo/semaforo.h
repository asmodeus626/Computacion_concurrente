#include <stdio.h>
#include "cola.h"

typedef struct {
    int value;
    struct Cola queue;
} semaphore;

void inicializar(semaphore *s, int val);
void wait(semaphore *s, int id, int arreglo[]);
void signal(semaphore *s, int arreglo[]);

void inicializar(semaphore *s, int val)
{
    s->queue.size = 0;
    s->value = val;
}

void wait(semaphore *s, int id, int arreglo[])
{
    s->value--;
    if (s->value < 0) {
        enqueue(&s->queue,id); //agrega este proceso a la cola de espera.
        arreglo[id] = 0; //bloquea al proceso
    }
}

void signal(semaphore *s, int arreglo[])
{
    s->value++;
    int prodes;
    if (s->value <= 0) {
        prodes = dequeue(&s->queue); //remueve un proceso en la cola
        arreglo[prodes] = 1;//despierta al proceso.
    }
}