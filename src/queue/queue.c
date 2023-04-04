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

void agregar_alfinal(struct Queue *queue, struct Process *process) {
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        return;
    }

    node->process = process;
    node->next = NULL;

    if (queue->head == NULL) {
        queue->head = node;
    } else {
        queue->tail->next = node;
    }

    queue->tail = node;
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
