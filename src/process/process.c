#include "process.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>

/*
void ordenar_fifo(struct Queue *fifo) {
    struct Node *nodo_actual, *nodo_siguiente, *nodo_temp;
    struct Process *proceso_actual, *proceso_siguiente;
    int intercambio = 1;

    while (intercambio) {
        intercambio = 0;
        nodo_actual = fifo->inicio;

        while (nodo_actual->siguiente != NULL) {
            nodo_siguiente = nodo_actual->siguiente;
            proceso_actual = nodo_actual->proceso;
            proceso_siguiente = nodo_siguiente->proceso;

            if (proceso_actual->orden > proceso_siguiente->orden) {
                // Intercambiar los nodos
                nodo_temp = nodo_actual;
                nodo_actual = nodo_siguiente;
                nodo_siguiente = nodo_temp;
                nodo_actual->anterior = nodo_temp->anterior;
                nodo_temp->anterior = nodo_actual;

                if (nodo_actual->siguiente != NULL) {
                    nodo_actual->siguiente->anterior = nodo_temp;
                }

                nodo_temp->siguiente = nodo_actual->siguiente;
                nodo_actual->siguiente = nodo_temp;
                intercambio = 1;
            }
            else {
                nodo_actual = nodo_siguiente;
            }
        }
    }
}

*/


struct Process *create_process(char *nombre, int tiempo_inicio, int burst, int io, char *path) {
    if (nombre == NULL || path == NULL) {
        printf("Error: el nombre o el path del proceso son nulos.\n");
        return NULL;
    }

    struct Process *process = (struct Process*) calloc(1, sizeof(struct Process));
    if (process == NULL) {
        printf("Error: no se pudo asignar memoria para el proceso.\n");
        return NULL;
    }

    process->nombre = calloc(strlen(nombre) + 1, sizeof(char));
    if (process->nombre == NULL) {
        printf("Error: no se pudo asignar memoria para el nombre del proceso.\n");
        free(process);
        return NULL;
    }

    strcpy(process->nombre, nombre);
    process->tiempo_inicio = tiempo_inicio;
    process->burst = burst;
    process->io = io;
    process->path = calloc(strlen(path) + 1, sizeof(char));
    if (process->path == NULL) {
        printf("Error: no se pudo asignar memoria para el path del proceso.\n");
        free(process->nombre);
        free(process);
        return NULL;
    }

    strcpy(process->path, path);
    //printf("proceso creado: nombre=%s, tiempo_inicio=%d, burst=%d, io=%d, path=%s\n", process->nombre, process->tiempo_inicio, process->burst, process->io, process->path);
    return process;
}

void destroy_process(struct Process* process) {
    free(process);
}


pid_t padre_pid;

void handle_sigcont(int signum) {
    // Manejar la señal SIGCONT
}

void handle_sigstop(int signum) {
    // Manejar la señal SIGSTOP
}

void handle_sigalrm(int signum) {
    kill(padre_pid, SIGSTOP);
}

void run_process(struct Process *proceso, char* path, char * const argv[]) {
    pid_t pid = fork();
    pid_t parent_pid = getppid();
    char *array_auxiliar[proceso->n_argumentos];
    if (pid == 0) {
        // el proceso hijito
        execv(path, array_auxiliar);
        exit(0);
    } else if (pid > 0) {
        // Proceso papá
        padre_pid = pid;
        int status;
        struct sigaction sa_cont, sa_stop, sa_alrm;

        sa_cont.sa_handler = handle_sigcont;
        sigemptyset(&sa_cont.sa_mask);
        sa_cont.sa_flags = 0;
        sigaction(SIGCONT, &sa_cont, NULL);

        sa_stop.sa_handler = handle_sigstop;
        sigemptyset(&sa_stop.sa_mask);
        sa_stop.sa_flags = 0;
        sigaction(SIGSTOP, &sa_stop, NULL);

        sa_alrm.sa_handler = handle_sigalrm;
        sigemptyset(&sa_alrm.sa_mask);
        sa_alrm.sa_flags = 0;
        sigaction(SIGALRM, &sa_alrm, NULL);

        // aca obtengo el tiempo, para calcular cpu
        int clock_cpu = 0;
        

        // Establecer alarma
        alarm(proceso->burst);

        while (1) {
            while(1){
                clock_cpu++;
            pid_t wpid = waitpid(pid, &status, WUNTRACED);

            if (wpid == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status)) {
                // Hijo terminó
                double elapsed_s = (double) clock_cpu / CLOCKS_PER_SEC;

                printf("Proceso %s terminado en %.6f segundos.\n", proceso->nombre, elapsed_s);
                proceso->turnaround = elapsed_s;
                proceso->pid = getppid();
                proceso->estado = 3;

                break;
            } else if (WIFSIGNALED(status)) {
                // Proceso hijo terminó por una señal
                break;
            } else if (WIFSTOPPED(status)) {
                // Proceso hijo detenido por SIGSTOP
                // Esperar a recibir SIGCONT para reanudar
                pause();
            }
        }
        }
    } else {
          // Error en fork()
        perror("fork");
        exit(EXIT_FAILURE);
    }
}


