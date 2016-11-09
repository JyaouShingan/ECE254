#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "msgqueue.h"


//Global variables
int total_producer;
int total_number;
int total_received;
msgqueue mqueue;
pthread_mutex_t queue_mux;
sem_t queue_items;
sem_t queue_spaces;


void *producer(void *arg) {
	//Get self producer id
	int p_id = *(int *)arg;
	free(arg);

	int curr = p_id;

	//Generate messages
	while (curr < total_number) {
		sem_wait(&queue_spaces);
		pthread_mutex_lock(&queue_mux);
		msgqueue_send(&mqueue, curr);
		pthread_mutex_unlock(&queue_mux);
		sem_post(&queue_items);
		curr += total_producer;
	}

	pthread_exit(0);
}

void *consumer(void *arg) {
	int c_id = *(int *)arg;
	free(arg);

	int value;

	while (total_received < total_number) {
		sem_wait(&queue_items);
		pthread_mutex_lock(&queue_mux);
		msgqueue_receive(&mqueue, &value);
		pthread_mutex_unlock(&queue_mux);
		total_received++;
		printf("%d %d %d", c_id, value, 0);
		sem_post(&queue_spaces);
	}

	pthread_exit(0);
}

int main(int argc, const char * argv[]) {
	int n, b, p, c;

	//Parse arguments
	if (argc < 5) {
		printf("4 arguments needed!\n");
		return -1;
	}
	n = atoi(argv[1]);
	b = atoi(argv[2]);
	p = atoi(argv[3]);
	c = atoi(argv[4]);
	total_number = n;
	total_producer = p;
	total_received = 0;

	//Thread pool && initialization
	pthread_t *p_pool = (pthread_t *)malloc(sizeof(pthread_t) * p);
	pthread_t *c_pool = (pthread_t *)malloc(sizeof(pthread_t) * c);
	msgqueue_init(&mqueue, b);
	pthread_mutex_init(&queue_mux, NULL);
	sem_init(&queue_items, 0, 0);
	sem_init(&queue_spaces, 0, b);

	//Spawn producers and consumers
	for (int i = 0; i < p; i++) {
		pthread_create(&p_pool[i], NULL, producer, (void *)i);
	}

	for (int i = 0; i < c; i++) {
		pthread_create(&c_pool[i], NULL, consumer, (void *)i);
	}

	for (int i = 0; i < p; i++) {
		pthread_join(p_pool[i], NULL);
	}

	for (int i = 0; i < c; i++) {
		pthread_join(c_pool[i], NULL);
	}

	//Clear up
	free(p_pool);
	free(c_pool);
	msgqueue_destory(&mqueue);
	pthread_mutex_destroy(&queue_mux);

    return 0;
}
