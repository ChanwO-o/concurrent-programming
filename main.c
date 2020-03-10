// Chan Woo Park 26984415 Min Sung Cha 85408485
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define BUFFERSIZE 8
#define INITBUFFERVALUE -1

int producercount;
int consumercount;
int items; // item count for each producer
int delay;

int buffer[BUFFERSIZE]; // buffer
int itemcount = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_cond_t isNotEmpty = PTHREAD_COND_INITIALIZER; // condition
pthread_cond_t isNotFull = PTHREAD_COND_INITIALIZER;

int in, out; // index of buffer for input/output

// threads
pthread_t producers[16];
pthread_t consumers[16];

void printBuffer() {
	int i;
	for (i = 0; i < BUFFERSIZE; ++i) {
		printf("%d  ", buffer[i]);
	}
	printf("\n");
}

void *produce(void *vargp) {
	int myid = (int) vargp;
	
	int i;
	for (i = 0; i < items; ++i) {
		pthread_mutex_lock(&lock);
		
		while (itemcount >= 8) {
			pthread_cond_wait(&isNotFull, &lock);
			// printf("p buffer is full; waiting %d\n", myid);
		}
	
		buffer[in] = i;
		itemcount++;
		in++;
		printf("producer_%d produced item %d\n", myid, i);
		// printBuffer();
		if (in == BUFFERSIZE)
			in = 0;
		
		pthread_cond_broadcast(&isNotEmpty);
		pthread_mutex_unlock(&lock);
		
		if (delay == 0)
			usleep( 1000 * 500 ); // 1 second
	}
}

void *consume(void *vargp) {
	int myid = (int) vargp;
	int consume_max = (producercount * items) / consumercount;
	
	int i;
	for (i = 0; i < consume_max; ++i) {
		pthread_mutex_lock(&lock);
			
		while (itemcount <= 0) {
			pthread_cond_wait(&isNotEmpty, &lock);
			// printf("c buffer is empty; waiting %d\n", myid);
		}
		
		buffer[out] = INITBUFFERVALUE;
		itemcount--;
		out++;
		printf("consumer_%d consumed item %d\n", myid % 100, i);
		
		if (out == BUFFERSIZE)
			out = 0;
		
		pthread_cond_broadcast(&isNotFull);
		pthread_mutex_unlock(&lock);
		
		if (delay == 1)
			usleep( 1000 * 500 );
	}
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
	}
}

void initializeConsumerThreads(int count) {
	int i;
	pthread_t tid = 100;
	for (i = 0; i < count; ++i) {
		pthread_create(&consumers[i], NULL, consume, tid);
		tid++;
	}
}

int main(int argc, char * argv[]) {
	if (argc != 5)
		return 1;
	
	producercount = atoi(argv[1]);
	consumercount = atoi(argv[2]);
	items = atoi(argv[3]);
	delay = atoi(argv[4]);
	
	initializeBuffer();
	initializeProducerThreads(producercount);
	initializeConsumerThreads(consumercount);
	
	int i;
	for (i = 0; i < producercount; ++i)
		pthread_join(producers[i], NULL); // only call this when thread is ready to be executed
	for (i = 0; i < consumercount; ++i)
		pthread_join(consumers[i], NULL);
	return 0;
}
