#include "shared_array.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <semaphore.h>

shared_array::shared_array(const std::string& name, size_t size)
    : name(name), size(size), owner(false)
{

    shm_fd = shm_open(name.c_str(), O_CREAT | O_RDWR | O_EXCL, 0666);
    if (shm_fd >= 0) {
        owner = true;
        ftruncate(shm_fd, size * sizeof(int));
    } else {
        shm_fd = shm_open(name.c_str(), O_RDWR, 0666);
        if (shm_fd < 0) {
            throw std::runtime_error("shm_open failed");
        }
    }

    data = static_cast<int*>(
        mmap(nullptr,
             size * sizeof(int),
             PROT_READ | PROT_WRITE,
             MAP_SHARED,
             shm_fd,
             0)
    );

    if (data == MAP_FAILED)
        throw std::runtime_error("mmap failed");


    std::string sem_name = name + "_sem";
    if (owner) {
        sem = sem_open(sem_name.c_str(), O_CREAT | O_EXCL, 0666, 1);
    } else {
        sem = sem_open(sem_name.c_str(), 0);
    }

    if (sem == SEM_FAILED)
        throw std::runtime_error("sem_open failed");

    if (owner) {
        sem_wait(sem);
        std::memset(data, 0, size * sizeof(int));
        sem_post(sem);
    }
}

shared_array::~shared_array()
{
    munmap(data, size * sizeof(int));
    close(shm_fd);
    sem_close(sem);

    if (owner) {
        shm_unlink(name.c_str());
        sem_unlink((name + "_sem").c_str());
    }
}

int& shared_array::operator[](size_t index)
{
    if (index >= size)
        throw std::out_of_range("Index out of range");

   sem_wait(sem);
   int& ref = data[index];
   sem_post(sem);

   return ref;
}

size_t shared_array::get_size() const
{
    return size;
}

