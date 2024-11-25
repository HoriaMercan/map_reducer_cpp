//
// Created by horia on 11/19/24.
//

#include "map_containers.h"

using std::vector;
using std::string;
using std::pair;

MapContainers::MapContainers(){
	containers = vector<vector<pair<std::string, unsigned>>>(
		__NUM_LETTERS, std::vector<std::pair<std::string, unsigned>>()
	);
}

SharedMapContainers::SharedMapContainers() {

	for (int i = 0; i < __NUM_LETTERS; i++) {
		sem_init(&semaphores[i], 0, 1);
	}

	pthread_mutex_init(&get_container_mutex, nullptr);

}

SharedMapContainers::~SharedMapContainers() {

	for (int i = 0; i < __NUM_LETTERS; i++) {
		sem_destroy(&semaphores[i]);
	}
	pthread_mutex_destroy(&get_container_mutex);
}

void MapContainers::AddWordFromFile(const std::string &word, unsigned int id) {
	if (word.empty()) return;
	containers[word[0] - 'a'].push_back(std::make_pair(word, id));
}

void SharedMapContainers::ConcatenateContainersOnLetter(Container_t &other_v, int letter) {
	auto &v = containers[letter];

	sem_wait(&semaphores[letter]);
	v.insert(v.end(), other_v.begin(), other_v.end());
	sem_post(&semaphores[letter]);
}

Container_t &MapContainers::getContainer(unsigned int index) {
	return static_cast<Container_t &>(containers[index]);
}

optional<std::pair<Container_t, unsigned>> SharedMapContainers::getNextContainer() {
	pthread_mutex_lock(&get_container_mutex);
	if (next_letter == __NUM_LETTERS) {
		pthread_mutex_unlock(&get_container_mutex);
		return optional<std::pair<Container_t, unsigned>>();
	}
	auto letter = next_letter++;

	pthread_mutex_unlock(&get_container_mutex);
	auto &a = containers[letter];

	auto pair = std::make_pair(a, letter);
	return optional<std::pair<Container_t, unsigned>>(pair);
}