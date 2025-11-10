#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>

typedef struct {
	sem_t queue;
	int waiting_threads;
} cv_t;

cv_t wait(cv_t cv) {
	cv.waiting_threads--;
	if(cv.waiting_threads < 0) {
		sem_wait(&cv.queue);
	}
	cv.waiting_threads++;
	return cv;
}

cv_t signal(cv_t cv) {
	cv.waiting_threads++;
	if(cv.waiting_threads > 0) {
		sem_post(&cv.queue);
	}
	cv.waiting_threads--;
	return cv;
}
