#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define BUFFERSIZE 8

// buffer
int buffer[BUFFERSIZE];
pthread_mutex_t lock; // mutex
int in, out; // index of buffer for input/output

// threads
pthread_t producers[16];
pthread_t consumers[16];

// if buffer is full or mutex is locked
// call mutex lock, start producing items
// calls mutex unlock
void *produce(void *vargp) {
	int myid = (int) vargp;
	printf("produce_id_%d\n", myid);
	usleep( 1000 * 1000 ); // 1 second
	return NULL;
}

void *consume(void *vargp) {
	int myid = (int) vargp;
	printf("consume_id_%d\n", myid);
	usleep( 1000 * 1000 ); // 1 second
	return NULL;
}

void initializeBuffer() {
	int i;
	for (i = 0; i < BUFFERSIZE; ++i)
		buffer[i] = 0;
}

void initializeProducerThreads(int count) {
	int i;
	pthread_t tid = 0;
	for (i = 0; i < count; ++i) {
		pthread_create(&producers[i], NULL, produce, tid);
		tid++;
		// pthread_join(producers[i], NULL); // only call this when thread is ready to be executed
	}
}

void initializeConsumerThreads(int count) {
	int i;
	pthread_t tid = 100;
	for (i = 0; i < count; ++i) {
		pthread_create(&consumers[i], NULL, consume, tid);
		tid++;
		// pthread_join(producers[i], NULL); // only call this when thread is ready to be executed
	}
}

int main(int argc, char * argv[]) {
	int producercount;
	int consumercount;
	int items;
	int delay;
	
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
	
	// pthread_join(producers[0], NULL); // only call this when thread is ready to be executed
	
	return 0;
}
