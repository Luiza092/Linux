#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]){
	if(argc != 3){
		std::cout << "Usage: " << argv[0] << " <source-file> <destination-file>" << std::endl;
		exit(EXIT_FAILURE);
	}

	const char* src = argv[1];
	const char* dest = argv[2];

	int s_fd = open(src, O_RDONLY);
	if(s_fd == -1){
		perror("error opening file");
		exit(EXIT_FAILURE);
	}

	int d_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(d_fd == -1){
		perror("error opening file");
		close(s_fd);
		exit(EXIT_FAILURE);
	}

	char buffer[4096];
	ssize_t bytes_read;
	while((bytes_read = read(s_fd, buffer, sizeof(buffer) - 1)) > 0){
		ssize_t bytes_written = write(d_fd, buffer, bytes_read);
		if(bytes_written == -1){
			perror("error writing file");
			close(s_fd);
			close(d_fd);
			exit(EXIT_FAILURE);
		}
	}

	if(bytes_read == -1){
		perror("error reading file");
		close(s_fd);
		close(d_fd);
		exit(EXIT_FAILURE);
	}

	close(s_fd);
	close(d_fd);
	return 0;
}
