#ifndef __TEMA1__SRC__RESOURCES__
#define __TEMA1__SRC__RESOURCES__

#include <pthread.h>
#include <queue>

#include "helpers.h"
#include "queue_element.h"
#include "../MapReduceIntermediary/map_containers.h"

class SharedResources{
public:
    SharedResources(unsigned num_mappers_, unsigned num_reducers_);

	~SharedResources();

	void WaitOnSharedBarrier();

	void AddTaskToQueue(queue_element_t element);

	// Thread-safe retrieval of a file info to be analysed by a mapper
	optional<queue_element_t> GetTask();

	SharedMapContainers &getSharedContainers();
private:
    unsigned num_mappers, num_reducers;

	// Barrier for synchronising mappers and reducers
	pthread_barrier_t barrier;

	// Queue for allocating tasks dynamically to the map workers
	std::queue<queue_element_t> map_workers_queue;

	// Mutex for synchronising
	pthread_mutex_t map_queue_mutex;

	// Container for intermediary results between map and reduce
	SharedMapContainers shared_containers;
};

#endif /*  __TEMA1__SRC__RESOURCES__  */