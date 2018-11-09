#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "semaforo.h"

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

	semaphore s1;
	inicializar(&s1, sem);
	
	//Sección paralela
	#pragma omp parallel private(i,id,j,sum,val)
	{
		id = omp_get_thread_num();

		for(i=0;i<10;i++)
		{
			//Esto es para asegurar que solo un hilo a la vez accede a la cola.
		    #pragma omp critical
			{
				wait(&s1, id, activo);
			}
			while(!activo[id]){} //Espera hasta que se reactive

			//Sección especial
			especial(id,s1.value);
    		//Termina la sección especial

			#pragma omp critical
			{
				signal(&s1, activo);
			}
		}
	}

	return 0;
}