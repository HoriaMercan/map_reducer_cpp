#include <fstream>
#include <iostream>

#include <vector>
#include <unordered_set>

#include "map_worker.h"
// #include "helpers.h"
#include "SharedResources/resources.h"

#define NUM_LETTERS ((unsigned)('z' - 'a' + 1))

using std::vector;
using std::unordered_set;

std::string normalise_word(std::string word) {
	std::string ans = "";
	for (const auto &c: word) {
		if (!isalpha(c)) continue;
		if (c >= 'A' && c <= 'Z') {
			ans += c - 'A' + 'a';
		} else {
			ans += c;
		}
	}
	return ans;
}

void map_loop(SharedResources * resources, std::string filename, unsigned id) {
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		std::cerr << "File " << filename << "does not exists\n";
		exit(1);
	} else {
//		std::cout << "Reading from " << filename << "\n";
	}

	/**
	 * We will define the following container:
	 * in each containers[i] (i = 0 ... NUM_LETTERS - 1) we
	 * will store all the pairs {word, fileID} where the word
	 * begins with the letter (char)(i + 'a')
	 *
	 * Such that, we can make fewer calls for updating the shared memory
	 * and we can reduce work for reducers in searching words starting
	 * with a certain letter
	 * */
	MapContainers localContainers;

	std::string word;
	while (fin >> word) {
		word = normalise_word(word);
		localContainers.AddWordFromFile(word, id);
	}
	fin.close();

//	std::cout << "Done reading from file " << filename << "\n";

	SharedMapContainers &sharedContainers = resources->getSharedContainers();
	for (unsigned i = 0; i < NUM_LETTERS; i++) {
		sharedContainers.ConcatenateContainersOnLetter(localContainers.getContainer(i), i);
	}
}

void* mapper_run(void * arg) {
	auto *resources = (SharedResources *)arg;

	optional<queue_element_t> elem_opt;
	while (elem_opt = resources->GetTask() ,
			elem_opt.has_value()) {
		auto elem = elem_opt.value();
		map_loop(resources, elem.filename, elem.file_id);
	}

	resources->WaitOnSharedBarrier();
	return nullptr;
}