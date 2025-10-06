#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
int main(int argc, char** argv){
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << std::endl;
		exit(1);
	}

	const char* filename = argv[1];

	int fd1 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd1 == -1){
		std::cerr << "Error opening file: " << strerror(errno) <<  std::endl;
		exit(1);
	}

	int fd2 = dup(fd1);
	if(fd2 == -1){
		std::cerr << "Error dublicating file: " << strerror(errno) << std::endl;
		close(fd1);
		exit(1);
	}

	const char* line1 = "first line\n";
	const char* line2 = "second line\n";

	int w1 = write(fd1, line1, strlen(line1));
	int w2 = write(fd2, line2, strlen(line2));

	if(w1 != (ssize_t)strlen(line1)){
		std::cerr << "Error writing first line" << std::endl;
	}

	if(w2 != (ssize_t)strlen(line2)){
		std::cerr << "rror writing second line" << std::endl;
	}

	close(fd1);
	close(fd2);
 	return 0;
}

