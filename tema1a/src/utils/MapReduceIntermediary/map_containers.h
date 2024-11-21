//
// Created by horia on 11/19/24.
//

#ifndef __TEMA1__SRC__MAPREDUCEINTERMEDIARY__
#define __TEMA1__SRC__MAPREDUCEINTERMEDIARY__

#include <vector>
#include <string>
#include <semaphore.h>
#include <pthread.h>
#include "helpers.h"

#define __NUM_LETTERS ((int )('z' - 'a' + 1))
typedef std::vector<std::pair<std::string, unsigned>> Container_t;

class MapContainers {
public:
	MapContainers();

	void AddWordFromFile(std::string word, unsigned int id);

	Container_t &getContainer(unsigned index);

protected:
	std::vector <std::vector<std::pair < std::string, unsigned>>>
	containers;
};

class SharedMapContainers : public MapContainers {
public:
	SharedMapContainers();

	~SharedMapContainers();

	/**
	 * This function inserts in the shared memory the container (the vector of
	 * pairs {word, fileID} from a map worker where word starts with a certain
	 * letter)
	 * Thread-safe function, multiple containers for different letters can be
	 * added at the same time
	 * */
	void ConcatenateContainersOnLetter(Container_t &other_v, int letter);


	/**
	 * This function retrieves from the shared memory into a reduce worker
	 * the vector of pairs {word, fileID} where word starts with the next
	 * following letter
	 * */
	optional<std::pair<Container_t, unsigned >> getNextContainer();

private:
	sem_t semaphores[__NUM_LETTERS];
	pthread_mutex_t get_container_mutex;
	unsigned next_letter = 0;

};

#endif /*  __TEMA1__SRC__MAPREDUCEINTERMEDIARY__  */