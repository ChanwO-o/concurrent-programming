#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define BUFFERSIZE 8
#define INITBUFFERVALUE -1

// buffer
int buffer[BUFFERSIZE];
pthread_cond_t isNotEmpty = PTHREAD_COND_INITIALIZER; // condition
pthread_cond_t isNotFull = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // mutex
int in, out; // index of buffer for input/output

// threads
pthread_t producers[16];
pthread_t consumers[16];

void printBuffer() {
	int i;
	for (i = 0; i < BUFFERSIZE; ++i) {
		printf("%d: %d\n", i, buffer[i]);
	}
}

int bufferIsFull() {
	int i;
	for (i = 0; i < BUFFERSIZE; ++i) {
		if (buffer[i] == INITBUFFERVALUE)
			return 0;
	}
	return 1;
}

int bufferIsEmpty() {
	int i;
	for (i = 0; i < BUFFERSIZE; ++i) {
		if (buffer[i] != INITBUFFERVALUE)
			return 0;
	}
	return 1;
}

void *produce(void *vargp) {
	pthread_mutex_lock(&lock);
	int myid = (int) vargp;
	printf("produce_id_%d\n", myid);
	
	if (bufferIsFull())
		pthread_cond_wait(&isNotFull, &lock);
	
	buffer[in] = 77;
	in++;
	if (in == BUFFERSIZE)
		in = 0;
	printBuffer();
	
	if (bufferIsEmpty() == 0)
		pthread_cond_signal(&isNotEmpty);
	
	pthread_mutex_unlock(&lock);
	usleep( 1000 * 1000 ); // 1 second
	return NULL;
}

void *consume(void *vargp) {
	pthread_mutex_lock(&lock);
	int myid = (int) vargp;
	printf("consume_id_%d\n", myid);
	
	if (bufferIsEmpty())
		pthread_cond_wait(&isNotEmpty, &lock);
	
	buffer[out] = INITBUFFERVALUE;
	out++;
	if (out == BUFFERSIZE)
		out = 0;
	printBuffer();
	
	if (bufferIsFull() == 0)
		pthread_cond_signal(&isNotFull);
	
	pthread_mutex_unlock(&lock);
	usleep( 1000 * 1000 );
	return NULL;
}

void initializeBuffer() {
	int i;
	for (i = 0; i < BUFFERSIZE; ++i)
		buffer[i] = INITBUFFERVALUE;
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
