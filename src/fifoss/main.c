#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

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

int main(int argc, char **argv){
	int numero_procesos, tiempo_inicio, burst, io, largo;
	char nombre[50], path[50];
    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    
	/* Creo la FIFO, inicialmente vacia*/

	struct Queue *fifo = crear_lista(); 

	/* Obtengo numero de procesos*/
	fscanf(input_file, "%d", &numero_procesos);
	printf("n procesos: %d\n", numero_procesos);
  
	/* Itero por los procesos para ir agregandolos a la lista ligada fifo*/
	for (int i = 0; i < numero_procesos; i++) {
        printf("loop: %d\n", i);
        fscanf(input_file, "%s %d %d %d %s ", nombre, &tiempo_inicio, &burst, &io, path);
        //printf("nombre: %s, tiempo_inicio: %d, burst: %d, io: %d, path: %s\n", nombre, tiempo_inicio, burst, io, path);
        fscanf(input_file, "%d",&largo);
        printf("largo: %d\n", largo);
        // ACA VER LO DE LOS INPUT
        char **argumentos = malloc(sizeof(char*) * (largo + 1));  // Reserva memoria para n + 2 argumentos
        for (int i = 0; i < largo; i++) {
          char *arg = malloc(sizeof(char) * 20);  // Reserva memoria para un argumento de longitud máxima MAX_ARG_LEN
          fscanf(input_file, "%s", arg);  // Lee el siguiente argumento del archivo
          argumentos[i] = arg;  // Agrega el argumento al array de argumentos
        }
      argumentos[largo] = NULL;  // Agrega un puntero NULL al final del array de argumentos  // Agrega otro puntero NULL al final del array de argumentos (requerido por execv)
// ...
        // Asegúrate de que el array de argumentos esté nulo-terminado
        // Llamado a execv

        struct Process *proceso = create_process(nombre, tiempo_inicio, burst, io, path); 
        proceso->n_argumentos = largo;
        proceso->estado = 0; /* Entran en estado ready*/
        proceso->primera_vez = 1;
        proceso->ingreso_waiting = proceso->tiempo_inicio;
        proceso->waiting_time = 0;
        proceso->argumentos = argumentos;
        
        agregar(fifo, proceso);
        /* Hasta aca, se agregaron a la fifo segun el orden de tiempo inicio.*/
  }
  
  //printf("la cabeza es : %s\n", fifo->head->process->nombre);       
  /* Parto un while*(1), que el scheduler corra todo el rato*/
  int clock = 0;
  while (1) {
    clock++;
    // constatemente reviso si un proceso tiene que pasar a ready
    check_waiting(fifo, clock);
  /* Reviso en la fifo si hay algun proceso que debe empezar*/
  /* Reviso en la fifo si hay algun proceso que debe empezar*/
    struct Node *current = fifo->head;
    //printf("la cabeza es : %s\n", fifo->head->process->nombre);
    while (current != NULL) {
      struct Process *process = current->process;
        if (process->estado == 0 && process->tiempo_inicio <= (double)(clock/CLOCKS_PER_SEC)) {
          printf("Proceso debe partir en segundo : %d\n", fifo->head->process->tiempo_inicio);
          // Ejecutar proceso, SIMULADO
          //run_process(current->process);
          // Le sumo a waiting time
          process->waiting_time += (double)(clock/CLOCKS_PER_SEC) - process->ingreso_waiting;
          printf("Proceso %s ejecutando en tiempo %F\n", process->nombre, (double)(clock/CLOCKS_PER_SEC));
          // Supongo que se ejecuto
          //cambio estado
          current->process->estado = 2;
          //aumenta numero veces que entra a cpu
          current->process->n_veces_cpu += 1;
          // si es primera vez que entra, calculo su response time.
          if (current->process->primera_vez){
            current->process->response_time = (double)((clock/CLOCKS_PER_SEC)-current->process->tiempo_inicio);
          }
          // Entra con tiempo waiting, para calcular waiting time
          current->process->ingreso_waiting = (double)(clock/CLOCKS_PER_SEC);
          mover_alfinal(fifo);
          break; // Salir del ciclo al encontrar un proceso a ejecutar
        }   
      current = current->next;
    }
    }

    
	return 0;

}