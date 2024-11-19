//
// Created by horia on 11/19/24.
//

#include "map_containers.h"

using std::vector, std::string, std::pair;

MapContainers::MapContainers(bool multithreaded_):
multithreaded(multithreaded_){
	containers = vector<vector<pair<std::string, unsigned>>>(
		NUM_LETTERS, std::vector<std::pair<std::string, unsigned>>()
	);

	if (multithreaded_) {
		semaphores = new sem_t[NUM_LETTERS];
		for (int i = 0; i < NUM_LETTERS; i++) {
			sem_init(&semaphores[i], 0, 1);
		}
	} else {
		semaphores = nullptr;
	}
}

MapContainers::~MapContainers() {
	if (multithreaded) {
		for (int i = 0; i < NUM_LETTERS; i++) {
			sem_destroy(&semaphores[i]);
		}
		delete []semaphores;
	}
}

void MapContainers::AddWordFromFile(std::string word, unsigned int id) {
	if (word.empty()) return;
	containers[word[0]].emplace_back(word, id);
}

void MapContainers::ConcatenateContainers(MapContainers &other) {

}