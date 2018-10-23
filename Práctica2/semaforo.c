#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "cola.h"

void especial(int idhilo, int csemaforo)
{
    printf("Hilo %d\n",idhilo);
    printf("Contador: %d\n",csemaforo);
    int i = 0;
    int sum;
    //Suma del 1 al 1000
    for(i=0;i<1000;i++)
    {
    	sum+=i;
    }
    
    printf("Suma = %d\n",sum);
    printf("El hilo %d sale de la sección especial\n",idhilo);
}

int main()
{
	int sem = 4; //4 hilos podrán acceder de forma simultánea a la sección especial
	int sum; //Resultado de la suma
	int id,i,j,numHilos,val;
	printf("Indica el número de hilos a ejecutar:\n");
	scanf("%d",&numHilos);
	omp_set_num_threads(numHilos);
	int activo[numHilos]; //Nos ayudará a verificar si un hilo está activo o dormido.
	int k;

	for(k=0;k<numHilos;k++)
	{
		activo[k] = 1; //Inicialmente todos estarán activos.
	}

	struct Cola cola1; //Cola donde se guardarán los procesos.
	cola1.size = 0;
	
	//Sección paralela
	#pragma omp parallel private(i,id,j,sum,val)
	{
		id = omp_get_thread_num();

		for(i=0;i<10;i++)
		{
			if(sem<=0)
			{
				//Esto es para asegurar que solo un hilo a la vez accede a la cola.
				#pragma omp critical
				{
					enqueue(&cola1,id); //Metemos el hilo a la cola.
				}
				activo[id]=0; //Desactivamos el hilo.
				while(!activo[id]){} //Espera hasta que se reactive
			}
			else
			{
				sem--;
			}

			//Sección especial
			especial(id,sem);
    		//Termina la sección especial

			if(isEmpty(&cola1))
			{
				sem++;
			}
			else
			{
				//Para asegurarnos de que solo 1 hilo accede a la cola.
				#pragma omp critical
				{
					val = dequeue(&cola1); //Desencolamos un hilo
				}
				activo[val] = 1; //Activamos el hilo
			}
		}
	}

	return 0;
}