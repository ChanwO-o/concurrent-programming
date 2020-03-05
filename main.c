#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define BUFFERSIZE 8

int buffer[BUFFERSIZE];
pthread_t *producers;
pthread_t *consumers;

void *thread(void *vargp) {
	printf("thread()\n");
	usleep( 1000 * 1000 ); // 1 second
	return NULL;
}

void initializeBuffer() {
	int i;
	for (i = 0; i < BUFFERSIZE; ++i)
		buffer[i] = 0;
}

void initializeProducerThreads(int count) {
	producers = (pthread_t *) malloc(sizeof(pthread_t) * count);
}

void initializeConsumerThreads(int count) {
	consumers = (pthread_t *) malloc(sizeof(pthread_t) * count);
}

int main(int argc, char * argv[])
{
	int producercount;
	int consumercount;
	int items;
	int delay;
	
	// int lock;
	// int currentThreadId;
	// int isFull, isEmpty;
	
	if (argc != 5)
		return 1;

	if (atoi(argv[1]) >= 1 && atoi(argv[1]) <= 16)
	{
		producercount = atoi(argv[1]);
	}
	if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 16 &&
		atoi(argv[2]) < (atoi(argv[1]) * atoi(argv[3])))
	{
		consumercount = atoi(argv[2]);
	}
	items = atoi(argv[3]);
	delay = atoi(argv[4]);

	printf("%d %d %d %d\n", producercount, consumercount, items, delay);
	
	initializeBuffer();
	initializeProducerThreads(producercount);
	initializeConsumerThreads(consumercount);
	
	
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	pthread_join(tid, NULL);

    return 0;
}
