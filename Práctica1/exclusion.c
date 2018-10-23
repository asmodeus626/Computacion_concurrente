#include<stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>

/*Función que suma los primeros 1000 números naturales. */
int suma1000()
{
	int i;
	int sum = 0;
	for(i=0;i<1000;i++)
	{
		sum+=i;
	}

	return sum;
}

/* Algoritmo de exclusión mutua de Dekker */
void dekker()
{
	printf("Algoritmo de Dekker\n\n");
	int turno = 0; //Variable compartida(todos los hilos ven la misma).
	int i,idHilo,resSuma; //Variables que se ocuparán en cada hilo.

	omp_set_num_threads(2); //Se ejecutarán 2 hilos.
	//En este momento se crean los hilos.
	#pragma omp parallel private(i,idHilo,resSuma) //Hay que especificar que esas variables son privadas para que se copien a cada hilo.
	{
		idHilo = omp_get_thread_num(); //Se obtiene el id del hilo.

		for(i=0;i<20;i++)
		{
			while(turno == 1-idHilo){} //Mientras sea turno del hilo contrario solo espera.

			printf("El hilo %d entró en la sección crítica\n",idHilo);
			resSuma = suma1000();
			printf("La suma del 1 al 1000 es: %d \n",resSuma);
			printf("El hilo %d terminó su sección crítica\n\n",idHilo);

			turno = 1-idHilo; //Le pasamos el turno al otro hilo.
		}
	}//Aquí se termina la ejecución paralela.
}


/*Algoritmo de Peterson*/
void peterson()
{
	printf("Algoritmo de Peterson\n\n");
	int turno; //Variable compartida.
	int bandera[2] = {0,0}; //Arreglo compartido.
	int i,idHilo,resSuma; //Variables que se ocuparán en cada hilo.
	omp_set_num_threads(2);

	#pragma omp parallel private(i,idHilo,resSuma)
	{
		idHilo = omp_get_thread_num();

		for(i=0;i<20;i++)
		{
			bandera[idHilo] = 1; //Le asigna la bandera a este hilo.
			turno = 1-idHilo; //Le asigna el turno al otro hilo.

			while(bandera[1-idHilo] && turno == 1-idHilo){} //Mientras la bandera y el turno sean del contrario espera.

			printf("El hilo %d entró en la sección crítica\n",idHilo);
			resSuma = suma1000();
			printf("La suma del 1 al 1000 es: %d \n",resSuma);
			printf("El hilo %d terminó su sección crítica\n\n",idHilo);

			bandera[idHilo] = 0;
		}
	}
}

/*Algoritmo de Kessels*/
void kessels()
{
	printf("Algoritmo de Kessels\n\n");
	int b[2]={0,0}; //Arreglo compartido
	int turno[2]={0,0};
	int local;
	int i,idHilo,resSuma;
	omp_set_num_threads(2);

	#pragma omp parallel private(i,idHilo,resSuma,local)
	{
		idHilo = omp_get_thread_num();

		for(i=0;i<20;i++)
		{
			b[idHilo] = 1; //Le asigna la bandera a este hilo.
			local = (idHilo + turno[1-idHilo])%2;
			turno[idHilo] = local;

			while(b[1-idHilo] && local==(turno[1-idHilo]+idHilo)%2){}

			printf("El hilo %d entró en la sección crítica\n",idHilo);
			resSuma = suma1000();
			printf("La suma del 1 al 1000 es: %d \n",resSuma);
			printf("El hilo %d terminó su sección crítica\n\n",idHilo);

			b[idHilo] = 0;
		}
	}
}

int main()
{
	dekker();
	peterson();
	kessels();
}