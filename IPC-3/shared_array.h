#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <string>
#include <semaphore.h>
#include <cstddef>
#include <exception>

class shared_array {
private:
	std::string name;
	size_t size;
	int* data;

	int shm_fd;
	sem_t* sem;
	bool owner;

public:
	shared_array(const std::string& name, size_t size);
	~shared_array();

	int& operator[](size_t index);
	size_t get_size() const;
};

#endif
