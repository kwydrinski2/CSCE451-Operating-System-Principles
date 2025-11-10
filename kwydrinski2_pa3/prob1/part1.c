#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int buffer_length;
int number_of_producers;
int number_of_consumers;
int number_of_items;

char item = 'X';
static int shared_resources = 0;

typedef struct {
	char *items;
	sem_t mutex, empty, full;
} buffer_t;

buffer_t buffer;

buffer_t init(buffer_t buffer) {
	sem_init(&buffer.mutex, 0, 1);
	sem_init(&buffer.full, 0, 0);
	sem_init(&buffer.empty, 0, buffer_length);

	buffer.items = (char *) malloc(sizeof(char) * buffer_length);
	return buffer;
}

int produce(int *threadpid) {
	buffer.items[shared_resources] = item;
	fflush(stdout);
	printf("p:<%u>, item: %c, at %d\n", *threadpid, item, shared_resources);
	shared_resources++;
	number_of_items--;
	return 0;
}

int consume(int *threadcid) {
	shared_resources--;
	char temp = buffer.items[shared_resources];
	buffer.items[shared_resources] = '\0';
	fflush(stdout);
	printf("c:<%u>, item: %c, at %d\n", *threadcid, temp, shared_resources);
	return 0;
}

void *producer(void *id) {
	int *threadpid = (int *) id;
	while(1) {
		sem_wait(&buffer.empty);
		sem_wait(&buffer.mutex);
		if(number_of_items > 0) {
			produce(threadpid);
			sem_post(&buffer.mutex);
			sem_post(&buffer.full);
		} else {
			sem_post(&buffer.mutex);
			sem_post(&buffer.full);
			return 0;
		}
	}
}

void *consumer(void *id) {
	int *threadcid = (int *) id;
	while(1) {
		sem_wait(&buffer.full);
		sem_wait(&buffer.mutex);
		if(shared_resources > 0) {
			consume(threadcid);
			sem_post(&buffer.mutex);
			sem_post(&buffer.empty);
		} else {
			sem_post(&buffer.mutex);
			sem_post(&buffer.empty);
			exit(0);
		}
	}
}

int main(int argc, char *argv[]) {
	int c;
	while ((c = getopt(argc, argv, ":b:p:c:i:")) != -1) {
		switch(c) {
			case 'b':
				buffer_length = atoi(optarg);
				break;
			case 'p':
				number_of_producers = atoi(optarg);
				break;
			case 'c':
				number_of_consumers = atoi(optarg);
				break;
			case 'i':
				number_of_items = atoi(optarg);
				break;
			case ':':
				printf("Missing Argument\n");
				break;
			case '?':
				printf("Unknown Argument\n");
				break;
		}
	}

	buffer = init(buffer);
	pthread_t producers[number_of_producers];
	pthread_t consumers[number_of_consumers];
	
	int pid_list[number_of_producers];
	int cid_list[number_of_consumers];

	for (int i = 0; i < number_of_producers; i++) {
		pid_list[i] = i + 1;
		pthread_create(&producers[i], NULL, producer, &pid_list[i]);
	}
	for (int i = 0; i < number_of_producers; i++) {
		cid_list[i] = i + 1;
		pthread_create(&consumers[i], NULL, consumer, &cid_list[i]);
	}
	for (int i = 0; i < number_of_producers; i++) {
		pthread_join(producers[i], NULL);
	}
	for (int i = 0; i < number_of_producers; i++) {
		pthread_join(consumers[i], NULL);
	}
	return 0;
}
