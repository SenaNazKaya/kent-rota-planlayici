#include "queue.h"
#include <stdlib.h>

Queue* queueOlustur() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

void queueEkle(Queue* q, int value) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = value;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

int queueCikar(Queue* q) {
    if (queueBosMu(q)) return -1;
    
    QueueNode* temp = q->front;
    int data = temp->data;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return data;
}

int queueBosMu(Queue* q) {
    return q->front == NULL;
}

void queueTemizle(Queue* q) {
    while (!queueBosMu(q)) {
        queueCikar(q);
    }
    free(q);
}

int queueBoyut(Queue* q) {
    return q->size;
}