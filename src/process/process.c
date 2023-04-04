#include "process.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    printf("proceso creado: nombre=%s, tiempo_inicio=%d, burst=%d, io=%d, path=%s\n", process->nombre, process->tiempo_inicio, process->burst, process->io, process->path);
    return process;
}

void destroy_process(struct Process* process) {
    free(process);
}