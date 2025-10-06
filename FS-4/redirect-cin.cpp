#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <algorithm>

void initialize(int argc, char** argv){
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
		exit(1);
	}

	const char* filename = argv[1];
	int fd = open(filename, O_RDONLY);
	if(fd == -1){
		std::cerr << "Error opening file :" << filename << strerror(errno) << std::endl;
		exit(1);
	}

	if(dup2(fd, 0) == -1){
		std::cerr << "redirect error: " << strerror(errno) << std::endl;
		close(fd);
		exit(1);
	}
	close(fd);
}

int main(int argc, char** argv){
	initialize(argc, argv);

	std::string input;
	std::cin >> input;
	if(input.empty()){
		std::cerr << "Input not found\n";
		exit(1);
	}

	std::reverse(input.begin(), input.end());
	std::cout << input <<std::endl;

	return 0;
}

