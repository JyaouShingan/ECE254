#ifndef msgqueue_h
#define msgqueue_h

#include <stdio.h>

typedef struct _msgqueue {
	int capacity;
	int count;
	int head;
	int tail;
	int *array;
} msgqueue;

int msgqueue_init(msgqueue *queue, int capacity);
int msgqueue_send(msgqueue *queue, int value);
int msgqueue_receive(msgqueue *queue, int *output);
int msgqueue_get_count(msgqueue* queue);
int msgqueue_get_capacity(msgqueue *queue);
void msgqueue_destory(msgqueue *queue);

#endif /* msgqueue_h */
