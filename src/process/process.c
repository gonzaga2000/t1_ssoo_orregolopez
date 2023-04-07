#include "process.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>







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



void handle_sigstop(int signum) {
    // Manejar la señal SIGSTOP
}

void handle_sigcont(int signum) {
    // Manejar la señal SIGCONT
    printf("Proceso hijo reanudado.\n");
}

void handle_sigalrm(int signum) {
    // Manejar la señal SIGALRM
    kill(padre_pid, SIGSTOP);
    printf("Tiempo de CPU para el proceso hijo ha terminado. Enviando SIGSTOP...\n");
    sleep(1); // Asegurarse de que el proceso hijo esté detenido antes de reanudarlo
    kill(padre_pid, SIGCONT);
}

void run_process(struct Process *proceso) {
    pid_t pid = fork();
    pid_t parent_pid = getpid(); // Corregido aquí, debe ser getpid() en vez de getppid()
    if (pid == 0) {
        // el proceso hijito
        char* arr[] = { proceso->path, NULL }; // Corregido aquí, se deben usar los argumentos originales
        execv(proceso->path, arr);
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
        clock_t start = clock(); // Agregado aquí, se guarda el tiempo de inicio

        // Establecer alarma
        int tiempo_restante = proceso->burst;
while (tiempo_restante > 0) {
    tiempo_restante--;
}

        while (1) {
            pid_t wpid = waitpid(pid, &status, WUNTRACED);

            if (wpid == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status)) {
                // Hijo terminó
                clock_t end = clock(); // Agregado aquí, se guarda el tiempo de finalización
                double elapsed_s = ((double) (end - start)) / CLOCKS_PER_SEC; // Calculado el tiempo transcurrido
                printf("Proceso %s terminado en %.6f segundos.\n", proceso->nombre, elapsed_s);
                proceso->turnaround = elapsed_s;
                proceso->pid = pid; // Corregido aquí, debe ser pid en vez de getppid()
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

            clock_cpu++; // Corregido aquí, se debe aumentar el tiempo de CPU en cada iteración del while
        }
    } else {
        // Error en fork()
        perror("fork");
        exit(EXIT_FAILURE);
    }
}


