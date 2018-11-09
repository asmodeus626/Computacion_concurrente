
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include "semaforo.h"

//Recibe el id del proceso.
void pensar(int idproc)
{
    printf("P: Filósofo %d pensando\n", idproc);

    sleep(2); //Se duerme por 2 segundos.

    printf("H: Filósofo %d tiene hambre\n", idproc);
}

//Recibe id del proceso
void comer(int idproc)
{
    printf("C: Filósofo %d comiendo\n", idproc);

    sleep(2); //Se duerme por 2 segundos.
}

//Función principal
int main()
{
    int id,i,numHilos,k;
    numHilos = 5; //Habrá 5 filósofos.
    omp_set_num_threads(numHilos);
    int activo[numHilos]; //Nos ayudará a verificar si un hilo está activo o dormido.
    semaphore palo[5]; //Representará a los palillos.
    semaphore lim; //Este semáforo limitará el número de filósofos que intentan comer.

	for(k=0;k<numHilos;k++)
	{
		activo[k] = 1; //Inicialmente todos estarán activos.
        inicializar(&palo[k], 1);
	}

    inicializar(&lim, 4); //Se podrán sentar 4 filósofos a la vez.

    //Sección paralela
	#pragma omp parallel private(i,id)
    {
        id = omp_get_thread_num();

        for(i=0;i<5;i++)
        {
            pensar(id);

            //Esto es para que la llamada a wait sea atómica.
		    #pragma omp critical
            {
                wait(&lim, id, activo); //Se permiten pasar 4 filósofos y se bloquea al 5°.
            }
            while(!activo[id]){} //Espera hasta que se reactive

		    #pragma omp critical
			{
				wait(&palo[id], id, activo); //Toma el palillo izquierdo
			}
			while(!activo[id]){} //Espera hasta que se reactive

            #pragma omp critical
			{
				wait(&palo[(id+1)%5], id, activo); //Toma el palillo derecho
			}
			while(!activo[id]){} //Espera hasta que se reactive

            comer(id);

            #pragma omp critical
			{
				signal(&palo[id], activo); //Libera el palillo izquierdo.
			}

            #pragma omp critical
			{
				signal(&palo[(id+1)%5], activo); //Libera el palillo derecho.
			}

            printf("T: El filósofo %d terminó de comer\n",id);

            #pragma omp critical
			{
				signal(&lim, activo); //Hacemos signal al semáforo principal.
			}
        }

        printf("M: El filósofo %d se retira de la mesa\n",id);
    }

    printf("Todos los filósofos terminaron de comer\n");

    return 0;
}