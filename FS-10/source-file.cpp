#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>
#include <algorithm>

int main(int argc, char** argv){
	if(argc != 3){
		std::cerr << "Usage: " << argv[0] << std::endl;
		exit(1);
	}

	const char* srcPath = argv[1];
	const char* dstPath = argv[2];

	int src = open(srcPath, O_RDONLY);
	if(src == -1){
		std::cerr << "Error opening source file: " << strerror(errno) << std::endl;
		exit(errno);
	}

	int dest = open(dstPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(dest == -1){
		std::cerr << "Error opening destination file: " << strerror(errno) << std::endl;
		close(src);
		exit(errno);
	}

	struct stat st;
	if(fstat(src, &st) < 0){
		std::cerr << "fstat error: " << strerror(errno) << std::endl;
		close(src);
		close(dest);
		exit(errno);
	}

	off_t offset = 0;
	off_t total_data = 0;
	off_t total_hole = 0;
	const size_t buf_size = 4096;
	char buffer[buf_size];

	while(offset < st.st_size){
		off_t data_start = lseek(src, offset, SEEK_DATA);
		if(data_start == -1){
			if(errno == ENXIO){
				total_hole += (st.st_size - offset);
				break;
			}
			std::cerr << "lseek(SEEK_DATA) errer: " << strerror(errno) << std::endl;
			close(src);
			close(dest);
			exit(errno);
		}

		if(offset  < data_start){
			off_t h_leng = data_start - offset;
			lseek(dest, h_leng, SEEK_CUR);
			total_hole += h_leng;
		}

		off_t hole_start = lseek(src, data_start, SEEK_HOLE);
		if(hole_start == -1){
			if(errno == ENXIO) hole_start = st.st_size;
			else{
				std::cerr << "lseek( SEEK_HOLE) error: " << strerror(errno) << std::endl;
				close(src);
				close(dest);
				exit(errno);
			}
		}

		off_t _read = hole_start - data_start;
		if(lseek(src, data_start, SEEK_SET) == -1){
			std::cerr << "lseek(SEEK_SET) error: " << strerror(errno) << std::endl;
			close(src);
			close(dest);
			exit(errno);
		}

		while(_read > 0){
			ssize_t chunk = std::min(_read, (off_t)buf_size);
			ssize_t n = read(src, buffer, chunk);
			if(n < 0){
				std::cerr << "error reading data\n";
				close(src);
				close(dest);
				exit(errno);
			}
			if(n == 0) break;
			ssize_t w = write(dest, buffer, n);
			if(w != n){
				std::cerr << "error writing data\n";
				close(src);
				close(dest);
				exit(errno);
			}
			_read -= n;
			total_data += n;
		}
		offset = hole_start;
	}

	close(src);
        close(dest);

	if(total_data < 0) total_data = 0;
	if(total_hole < 0) total_hole = 0;

	std::cout << "Successfully copied " << st.st_size
		  << " bytes (data: " << total_data << ", hole: "
		  << ")." << std::endl;

	return 0;

}
