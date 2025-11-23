#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

struct ThreadData {
	const int* arr;
	size_t start;
	size_t end;
	long long* results;
	size_t id;
};

void* threadFunc(void* arg){
	ThreadData* data = static_cast<ThreadData*>(arg);

	long long sum = 0;
	for(size_t i = data->start; i < data->end; ++i) {
		sum += data->arr[i];
	}

	data->results[data->id] = sum;
	return nullptr;
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

	int* arr = new int[N];
	long long* partial_sums = new long long[M];

	for(size_t i = 0; i < N; ++i) {
		arr[i] = rand() % 100;
	}

	clock_t start_time = clock();

	long long single_sum = 0;
	for(size_t i = 0; i < N; ++i) {
		single_sum += arr[i];
	}

	clock_t end_time = clock();
	double duration_single = double(end_time - start_time) / CLOCKS_PER_SEC;

	pthread_t* threads = new pthread_t[M];
	ThreadData* tdata = new ThreadData[M];

	size_t chunk = N / M;
	start_time = clock();

	for(size_t i = 0; i < M; ++i) {
		tdata[i].arr = arr;
		tdata[i].start = i * chunk;
		tdata[i].end = (i == M - 1) ? N : (i + 1) * chunk;
		tdata[i].results = partial_sums;
		tdata[i].id = i;

		pthread_create(&threads[i], nullptr, threadFunc, &tdata[i]);
	}

	for(size_t i = 0; i < M; ++i) {
		pthread_join(threads[i], nullptr);
	}

	end_time = clock();
	double duration_threads = double(end_time - start_time) / CLOCKS_PER_SEC;

	long long threaded_sum = 0;
	for(size_t i = 0; i < M; ++i) {
		threaded_sum += partial_sums[i];
	}

	std::cout << "Spent time without threads: " << duration_single << "s\n";
	std::cout << "Spent time with " << M << " threads: " << duration_threads << "s\n";

	if(single_sum != threaded_sum) {
		std::cerr << "Error: sums dont match\n";
	}

	delete[] arr;
	delete[] partial_sums;
	delete[] threads;
	delete[] tdata;

	return 0;
}
