#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <queue>
#include <vector>
#include <pthread.h>

class parallel_scheduler {
public:
	explicit parallel_scheduler(int capacity);

	~parallel_scheduler();

	void run(void (*func)(void*), void* arg);

private:
	struct Func {
		void (*func)(void*);
		void* arg;
	};

	void worker_loop();
	static void* worker_start(void* arg);

	int capacity;
	bool stop;

	std::vector<pthread_t> threads;
	std::queue<Func> funcs;

	pthread_mutex_t queue_mutex;
	pthread_cond_t queue_cond;
};

#endif

