#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <set>

#include "helpers.h"
#include "reduce_worker.h"
#include "SharedResources/resources.h"


/**
 * In this function we will analyze a container (list of
 * pairs {word, fileID}) which contains all the words in all
 * the texts starting with the letter 'a' + letter.
 * After aggregating the data, we will write them in
 * the required file.
 */
void reducer_loop(Container_t &container, int letter) {
	
	/**
	 * We will aggregate all the data such that we should
	 * get a map from a word to a list of file ids.
	 * {word, <ordered list of fileIDs>}
	 */
	std::unordered_map<std::string, std::set<unsigned>> ans_;
	for (const auto &in_container: container) {
		auto word = in_container.first;
		auto file = in_container.second;

		auto it = ans_.find(word);
		if (it == ans_.end()) {
			ans_[word] = std::set<unsigned>{file};
		} else {
			it->second.insert(file);
		}
	}

	/**
	 * Ordering function between {word, <ordered list of fileIDs>} type
	 * of pairs, such that we can organize them in the desired order.
	 */
	auto order_func = [](const std::pair<std::string, std::set<unsigned>> &a,
						 const std::pair<std::string, std::set<unsigned>> &b) {
		if (a.second.size() == b.second.size()) {
			return a.first < b.first;
		}
		return a.second.size() > b.second.size();
	};

	/**
	 * We will build a set using the abovementioned function
	 * to keep the words and lists in the desired order.
	 */
	std::set<std::pair<std::string, std::set<unsigned>>, decltype(order_func)>
		ans(order_func);

	for (const auto &in_ans: ans_) {
		ans.emplace(in_ans.first, std::move(in_ans.second));
	}

	/**
	 * Output in the file.
	 */
	std::string filename = "";
	filename += (char)('a' + letter);
	filename += ".txt";
	std::ofstream fout(filename);

	for (const auto &in_ans: ans) {
		auto k = in_ans.first;
		auto v = in_ans.second;

		fout << k << ":[";
		std::set<unsigned>::iterator it;
		for (it = v.begin(); it != std::prev(v.end()); it++) {
			fout << *it << " ";
		}
		fout << *it << "]\n";
	}
	fout.flush();
	fout.close();
}

/**
 * General structure of a reduce worker function
 */
void *reducer_run(void *arg)
{
	auto *resources = (SharedResources *) arg;
	resources->WaitOnSharedBarrier();

	optional<std::pair<Container_t, unsigned >> elem_opt;
	while (elem_opt = resources->getSharedContainers().getNextContainer(),
	elem_opt.has_value()) {
		auto elem = elem_opt.value();

		auto container = elem.first;
		auto letter = elem.second;

		reducer_loop(container, letter);
	}
	return nullptr;
}