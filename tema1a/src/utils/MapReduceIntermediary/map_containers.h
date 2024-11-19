//
// Created by horia on 11/19/24.
//

#ifndef __TEMA1__SRC__MAPWORKER__
#define __TEMA1__SRC__MAPWORKER__

#include <vector>
#include <string>
#include <semaphore.h>

#define NUM_LETTERS ((int )('z' - 'a' + 1))


class MapContainers {
public:
	MapContainers(bool multithreaded_);
	~MapContainers();
	void AddWordFromFile(std::string word, unsigned int id);
	void ConcatenateContainersOnLetter(MapContainers &other, int letter);
private:
	sem_t *semaphores;
	std::vector<std::vector<std::pair<std::string, unsigned>>> containers;
	bool multithreaded;
};

#endif /*  __TEMA1__SRC__MAPWORKER__  */