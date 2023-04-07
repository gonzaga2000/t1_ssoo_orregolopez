#include "queue.h"
#include "../process/process.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Queue *crear_lista() {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}


void check_waiting(struct Queue *fifo, int clock) {
    struct Node *current = fifo->head;
    while (current != NULL) {
        struct Process *proceso = current->process;
        if ((double)(clock/CLOCKS_PER_SEC)>= current->process->ingreso_waiting + current->process->io) {
            // Calculamos el tiempo de ingreso a ready
            proceso->estado = 0;
        } else {
            current = current->next;
        }
    }
}




void mover_alfinal(struct Queue *fifo) {
    // si hay un solo eleento en la fifo, no se hace nada
    if (fifo->head == fifo->tail) return;

    // se guarda el nodo a mover (head)
    struct Node *nodo_a_mover = fifo->head;

    // se actualiza el puntero head
    fifo->head = nodo_a_mover->next;

    // se actualiza el puntero tail
    fifo->tail->next = nodo_a_mover;
    nodo_a_mover->next = NULL;
    fifo->tail = nodo_a_mover;
}

void agregar(struct Queue *queue, struct Process *process) {
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        return;
    }

    node->process = process;
    node->next = NULL;

    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        // Buscar el lugar para insertar el nuevo proceso
        struct Node *current = queue->head;
        struct Node *previous = NULL;
        while (current != NULL && current->process->tiempo_inicio <= process->tiempo_inicio) {
            previous = current;
            current = current->next;
        }
        if (previous == NULL) { // Insertar en la cabeza de la cola
            node->next = queue->head;
            queue->head = node;
        } else { // Insertar en medio o al final de la cola
            node->next = current;
            previous->next = node;
            if (current == NULL) { // Si se inserta al final, actualizar la cola
                queue->tail = node;
            }
        }
    }
}

void eliminar(struct Queue *queue) {
    struct Node *node = queue->head;
    struct Process *process = node->process;
    queue->head = node->next;
    free(node);

    if (queue->head == NULL) {
        queue->tail = NULL;
    }
}
