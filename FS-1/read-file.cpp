#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]){
	if(argc != 2){
		std::cout << "Usage: " << argv[0] << " <file-path>" << std::endl;
		exit(EXIT_FAILURE);
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("error opening file");
		exit(EXIT_FAILURE);
	}

	char buffer[4096];
	ssize_t bytes_read;

	while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0){
		ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
		if(bytes_written == -1){
			perror("error writing stdout");
			close(fd);
			exit(EXIT_FAILURE);
		}
	}

	if(bytes_read == -1){
		perror("error reading file");
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(fd);
	return 0;
}
