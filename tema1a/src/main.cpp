#include <iostream>
#include <fstream>

#include <pthread.h>

#include "utils/SharedResources/resources.h"
#include "utils/MapWorker/map_worker.h"
#include "utils/ReduceWorker/reduce_worker.h"

int main(int argc, const char *argv[]) {
    unsigned M, R;

    if (argc != 4) {
        std::cerr << "Usage: ./tema1 <no of map workers> <no of reduce workers> <file name>\n";
        exit(-1);
    }
    sscanf(argv[1], "%u", &M);
    sscanf(argv[2], "%u", &R);

    std::ifstream fin(argv[3]);

    if (!fin.is_open()) {
        std::cerr << "File " << argv[3] << " does not exist.\n";
        exit(-1);
    }
	SharedResources resources(M, R);

	unsigned no_of_files;
	std::string filename;
	fin >> no_of_files;
	for (unsigned i = 0; i < no_of_files; i++) {
		fin >> filename;
		resources.AddTaskToQueue(queue_element_t(filename, i + 1));
	}
	fin.close();
	auto *threads = new pthread_t[M + R];
	for (unsigned i = 0; i < M; i++) {
		pthread_create(&threads[i], nullptr, mapper_run, &resources);
	}
	for (unsigned i = 0; i < R; i++) {
		pthread_create(&threads[i + M], nullptr, reducer_run, &resources);
	}


	for (unsigned i = 0; i < M + R; i++) {
		void *ans;
		pthread_join(threads[i], &ans);
	}
	delete []threads;
    return 0;
}