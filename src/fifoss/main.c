#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <string.h>
#include <stdbool.h>


bool check_arguments(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    exit(1);
  }
  return true;
}

int main(int argc, char **argv)
{
	int numero_procesos, tiempo_inicio, burst, io, largo;
	char nombre[50], path[50], argumentos[50];
	nombre[0] = '\0';
	path[0] = '\0';

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    
	/* Creo la FIFO, inicialmente vacia*/

	struct Queue *fifo = crear_lista(); 

	/* Obtengo numero de procesos*/
	fscanf(input_file, "%d", &numero_procesos);
	printf("n proceso: %d\n", numero_procesos);
	/* Itero por los procesos para ir agregandolos a la lista ligada fifo*/
	for (int i = 0; i < numero_procesos; i++) {
        printf("loop: %d\n", i);
        fscanf(input_file, "%s %d %d %d %s", nombre, &tiempo_inicio, &burst, &io, path);
        printf("nombre: %s, tiempo_inicio: %d, burst: %d, io: %d, path: %s\n", nombre, tiempo_inicio, burst, io, path);
        fscanf(input_file, "%d",&largo);
        printf("largo: %d\n", largo);
        if( largo != 0){
            fscanf(input_file, "%s",argumentos);
            printf("argumentos: %s\n", argumentos);
        }
        struct Process *proceso = create_process(nombre, tiempo_inicio, burst, io, path); 
        proceso->n_argumentos = largo;
        proceso->estado = 0; /* Entran en estado ready*/
        if(largo != 0){
          proceso->argumentos = argumentos;
        }
        agregar(fifo, proceso);
        printf("elemento head: %s\n", fifo->head->process->argumentos);
        printf("elemento tail: %s\n", fifo->tail->process->argumentos);

        /* Hasta aca, se agregaron a la fifo segun el orden de tiempo inicio.*/
	}
    
	return 0;

}
