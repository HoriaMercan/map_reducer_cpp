#include "resources.h"

#include <iostream>
#include <pthread.h>

SharedResources::SharedResources(unsigned num_mappers_, unsigned num_reducers_):
    num_mappers(num_mappers_), num_reducers(num_reducers_) {
	pthread_barrier_init(&barrier, nullptr, num_mappers_ + num_reducers_);
	pthread_mutex_init(&map_queue_mutex, nullptr);
}

SharedResources::~SharedResources()
{
	std::cout << "Destroying shared resources...\n";
	pthread_barrier_destroy(&barrier);
	pthread_mutex_destroy(&map_queue_mutex);
}

void SharedResources::WaitOnSharedBarrier() {
	pthread_barrier_wait(&barrier);
}

void SharedResources::AddTaskToQueue(queue_element_t element) {
	map_workers_queue.push(element);
}

optional<queue_element_t> SharedResources::GetTask() {
	pthread_mutex_lock(&map_queue_mutex);
	if (map_workers_queue.empty()) {
		pthread_mutex_unlock(&map_queue_mutex);
		return optional<queue_element_t>();
	} else {
		auto ans = optional<queue_element_t>(map_workers_queue.front());
		map_workers_queue.pop();
		pthread_mutex_unlock(&map_queue_mutex);
		return ans;
	}
}
SharedMapContainers& SharedResources::getSharedContainers() {
	return static_cast<SharedMapContainers &>(shared_containers);
}