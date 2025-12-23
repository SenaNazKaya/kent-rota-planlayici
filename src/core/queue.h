#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode {
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

// Kuyruk fonksiyonları
Queue* queueOlustur();
void queueEkle(Queue* q, int value);
int queueCikar(Queue* q);
int queueBosMu(Queue* q);
void queueTemizle(Queue* q);
int queueBoyut(Queue* q);

#endif