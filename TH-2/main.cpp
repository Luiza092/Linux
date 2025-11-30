#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

void task(void* arg) {
	int id = *static_cast<int*>(arg);
	std::cout << "Task " << id << " started\n";
	sleep(1);
	std::cout << "Task " << id << " finished\n";
}

int main() {
	parallel_scheduler pool(3);

	int task_ids[10];
	for(int i = 0; i < 10; ++i) {
		task_ids[i] = i;
		pool.run(task, &task_ids[i]);
	}

	sleep(5);
	return 0;
}
