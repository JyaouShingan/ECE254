#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "msgqueue.h"


//Global variables
int total_producer;
int total_number;
int total_received;
msgqueue mqueue;
pthread_mutex_t queue_mux;
pthread_mutex_t incre_mux;
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
	
	//Trying to received a message
	while (total_received < total_number) {
		sem_wait(&queue_items);
		//If we've already received all, flush the remaining consumer
		if (total_received >= total_number) {
			sem_post(&queue_items);
			pthread_exit(0);
		}
		pthread_mutex_lock(&queue_mux);
		msgqueue_receive(&mqueue, &value);
		pthread_mutex_unlock(&queue_mux);
		pthread_mutex_lock(&incre_mux);
		total_received++;
		pthread_mutex_unlock(&incre_mux);
		sem_post(&queue_spaces);
		//All number recevied, flush remaining consumer
		if (total_received >= total_number) sem_post(&queue_items);
		int root = sqrt(value);
		if (root * root == value ) {
			printf("%d %d %d\n", c_id, value, root);
		}
	}

	pthread_exit(0);
}

int main(int argc, const char * argv[]) {
	int n, b, p, c;
	struct timeval tv;
	double t1, t2;	

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
	pthread_mutex_init(&incre_mux, NULL);
	sem_init(&queue_items, 0 ,0);
	sem_init(&queue_spaces, 0, b);

	//Spawn producers and consumers
	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;

	for (int i = 0; i < p; i++) {
		int *arg = (int *)malloc(sizeof(int));
		*arg = i;
		pthread_create(&p_pool[i], NULL, producer, (void *)arg);
	}

	for (int i = 0; i < c; i++) {
		int *arg = (int *)malloc(sizeof(int));
		*arg = i;
		pthread_create(&c_pool[i], NULL, consumer, (void *)arg);
	}

	for (int i = 0; i < p; i++) {
		pthread_join(p_pool[i], NULL);
	}

	for (int i = 0; i < c; i++) {
		pthread_join(c_pool[i], NULL);
	}

	gettimeofday(&tv, NULL);
	t2 = tv.tv_sec + tv.tv_usec/1000000.0;

	printf("System execution time: %.6lf seconds\n", t2-t1);

	//Clear up
	free(p_pool);
	free(c_pool);
	msgqueue_destory(&mqueue);
	pthread_mutex_destroy(&queue_mux);
	pthread_mutex_destroy(&incre_mux);
	sem_destroy(&queue_items);
	sem_destroy(&queue_spaces);

    return 0;
}
