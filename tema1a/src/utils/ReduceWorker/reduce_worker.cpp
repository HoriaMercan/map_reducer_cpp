#include <iostream>

#include "reduce_worker.h"
#include "../SharedResources/resources.h"

void* reducer_run(void * arg) {
	auto *resources = (SharedResources *)arg;
	resources->WaitOnSharedBarrier();

	std::cout << "Hello from reduce!\n";
	return nullptr;
}