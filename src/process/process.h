#pragma once
#include <stddef.h>
#include <string.h>

struct Process {
    char *nombre;
    int tiempo_inicio;
    int pid;
    int estado;
    int burst;
    int io;
    char *path;
    char *argumentos;
    int n_argumentos;
};

struct Process *create_process(char *nombre, int tiempo_inicio, int burst, int io, char *path);


void destroy_process(struct Process* process);