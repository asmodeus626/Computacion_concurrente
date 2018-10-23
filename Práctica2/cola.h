#include <stdio.h>

struct Cola
{
	int size;
	int arreglo[100];
};

void enqueue(struct Cola *queue,int elem);
int dequeue(struct Cola *queue);
int peek(struct Cola *queue);
int isEmpty(struct Cola *queue);

void enqueue(struct Cola *queue,int elem)
{
	queue->arreglo[queue->size] = elem;
	queue->size++;
}

int dequeue(struct Cola *queue)
{
	int retVal = queue->arreglo[0]; //Le asigna el primer elemento del arreglo.
	//Recorremos el resto de los elementos una posición a la izquierda
	int i = 0;
	for(i = 0;i<queue->size-1;i++)
	{
		queue->arreglo[i] = queue->arreglo[i+1];
	}
	queue->arreglo[queue->size-1] = 0;
	queue->size--;

	return retVal;
}

int peek(struct Cola *queue)
{
	return queue->arreglo[0];
}

int isEmpty(struct Cola *queue)
{
	return queue->size==0;
}
