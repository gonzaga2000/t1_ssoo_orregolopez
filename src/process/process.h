#pragma once
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "../queue/queue.h"

struct Process {
    char *nombre;
    int tiempo_inicio;
    int pid;
    int orden;
    int estado;
    double ingreso_waiting;
    double waiting_time;
    double response_time;
    double turnaround;
    int primera_vez;
    int n_veces_cpu;
    int burst;
    int io;
    char *path;
    char **argumentos;
    int n_argumentos;
};

struct Process *create_process(char *nombre, int tiempo_inicio, int burst, int io, char *path);
void run_process(struct Process *proceso);
