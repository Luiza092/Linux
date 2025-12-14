#include "shared_array.h"
#include <iostream>
#include <unistd.h>

int main()
{
	shared_array arr("ipc_array", 5);

	int counter = 1;
	while (true) {
		for (size_t i = 0; i < arr.get_size(); i++) {
			arr[i] = counter++;
		}

		std::cout << "[FIRST] updated array" << std::endl;
		sleep(1);
	}
}
