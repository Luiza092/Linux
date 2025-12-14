#include "shared_array.h"
#include <iostream>
#include <unistd.h>

int main()
{
	shared_array arr("ipc_array", 5);

	while (true) {
		std::cout << "[SECOND] array values: ";
		for (size_t i = 0; i < arr.get_size(); i++) {
			std::cout << arr[i] << " ";
		}
		std::cout << std::endl;

		sleep(1);
	}
}
