#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>

void partial_sum(const std::vector<int>& arr, size_t start, 
	size_t end, long long& result){
	long long sum = 0;
	for(size_t i = start; i < end; ++i) {
		sum += arr[i];
	}
	result = sum;
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		std::cerr << "USAGE: " << argv[0] << "N M\n";
		return 1;
	}

	size_t N = std::atoi(argv[1]);
	size_t M = std::atoi(argv[2]);

	if(N < 1000000 || M < 1) {
		std::cerr << "N must be > 1000000 and M >= 1\n";
		return 1;
	}

	std::vector<int> arr(N);
	std::srand(std::time(nullptr));
	for(size_t i = 0; i < N; ++i) {
		arr[i] = rand() % 100;
	}

	clock_t start_time = clock();
	long long total_sum = 0;
	for(size_t i = 0; i < N; ++i) {
		total_sum += arr[i];
	}
	clock_t end_time = clock();
	double duration_single = double(end_time - start_time) /
		 CLOCKS_PER_SEC;

	std::vector<std::thread> threads;
	std::vector<long long> partial_sums(M, 0);

	size_t chunk_size = N / M;
	start_time = clock();
	for(size_t i = 0; i < M; ++i) {
		size_t start = i * chunk_size;
		size_t end = (i == M - 1) ? N : start + chunk_size;
		threads.emplace_back(partial_sum, std::ref(arr), start,
			 end, std::ref(partial_sums[i]));
	}

	for(auto& t : threads) t.join();

	long long total_sum_threads = 0;
	for(auto s : partial_sums) total_sum_threads += s;
	end_time = clock();
	double duration_multi = double(end_time - start_time)
		 / CLOCKS_PER_SEC;

	std::cout << "Time spent without threads: " << duration_single << "s\n";
	std::cout << "Time spent with " << M << " threads: " 
		 << duration_multi << "s\n";

	if(total_sum != total_sum_threads) {
		std::cerr << "Error: sums don't match!\n";
	}

	return 0;
}

