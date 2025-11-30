#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

parallel_scheduler::parallel_scheduler(int cap) : capacity(cap), stop(false) {
	pthread_mutex_init(&queue_mutex, nullptr);
	pthread_cond_init(&queue_cond, nullptr);

	threads.resize(capacity);
	for(int i = 0; i < capacity; ++i) {
		pthread_create(&threads[i], nullptr, worker_start, this);
	}
}

parallel_scheduler::~parallel_scheduler() {
	pthread_mutex_lock(&queue_mutex);
	stop = true;
	pthread_mutex_unlock(&queue_mutex);
	pthread_cond_broadcast(&queue_cond);

	for(int i = 0; i < capacity; ++i) {
		pthread_join(threads[i], nullptr);
	}

	pthread_mutex_destroy(&queue_mutex);
	pthread_cond_destroy(&queue_cond);
}

void parallel_scheduler::run(void (*func)(void*), void* arg) {
	pthread_mutex_lock(&queue_mutex);
	funcs.push({func, arg});
	pthread_mutex_unlock(&queue_mutex);

	pthread_cond_signal(&queue_cond);
}

void* parallel_scheduler::worker_start(void* arg) {
	parallel_scheduler* pool = static_cast<parallel_scheduler*>(arg);
	pool->worker_loop();
	return nullptr;
}

void parallel_scheduler::worker_loop() {
	while(true) {
		pthread_mutex_lock(&queue_mutex);
		while(!stop && funcs.empty()) {
			pthread_cond_wait(&queue_cond, &queue_mutex);
		}
		if(stop && funcs.empty()) {
			pthread_mutex_unlock(&queue_mutex);
			return;
		}

		Func f = funcs.front();
		funcs.pop();
		pthread_mutex_unlock(&queue_mutex);

		f.func(f.arg);
	}
}


