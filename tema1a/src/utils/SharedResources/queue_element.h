//
// Created by horia on 11/19/24.
//

#ifndef __TEMA1__SRC__RESOURCES__QUEUEELEMENT_H__
#define __TEMA1__SRC__RESOURCES__QUEUEELEMENT_H__

#include <string>

struct queue_element_t {
	std::string filename;

	unsigned file_id;

	queue_element_t() {};
	queue_element_t(std::string filename, unsigned file_id_) :
	filename(filename), file_id(file_id_) {};
};

#endif //__TEMA1__SRC__RESOURCES__QUEUEELEMENT_H__
