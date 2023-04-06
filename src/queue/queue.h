#pragma once

#include <stddef.h>
#include <stdlib.h>
#include "../process/process.h"
#include <time.h>

struct Node {
    struct Process* process;
    struct Node* next;
};

struct Queue {
    struct Node *head;
    struct Node *tail;
};


struct Queue *crear_lista();
void eliminar(struct Queue *queue);
void agregar(struct Queue *queue, struct Process *process);
void agregar_alfinal(struct Queue *queue, struct Process *proceso);
void check_waiting(struct Queue *fifo, int tiempo);
void ordenar_fifo(struct Queue *fifo);


