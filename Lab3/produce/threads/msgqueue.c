#include "msgqueue.h"
#include <stdlib.h>

int msgqueue_init(msgqueue *queue, int capacity) {
	if (queue == NULL) return -1;
	queue->capacity = capacity;
	queue->count = 0;
	queue->head = queue->tail = 0;
	queue->array = (int *)malloc(sizeof(int) * capacity);
	return 0;
}

int msgqueue_send(msgqueue *queue, int value) {
	if (queue == NULL) return -1;
	if (queue->count == queue->capacity) return -3;
	queue->count++;
	queue->array[queue->tail] = value;
	queue->tail = (queue->tail + 1) % queue->capacity;
	return 0;
}

int msgqueue_receive(msgqueue *queue, int *output) {
	if (queue == NULL) return -1;
	if (output == NULL) return -2;
	if (queue->count == 0) return -3;
	queue->count--;
	*output = queue->array[queue->head];
	queue->head = (queue->head + 1) % queue->capacity;
	return 0;
}

int msgqueue_get_count(msgqueue* queue) {
	if (queue == NULL) return -1;
	return queue->count;
}

int msgqueue_get_capacity(msgqueue *queue) {
	if (queue == NULL) return -1;
	return queue->capacity;
}

void msgqueue_destory(msgqueue *queue) {
	if (queue == NULL) return;
	free(queue->array);
	return;
}
