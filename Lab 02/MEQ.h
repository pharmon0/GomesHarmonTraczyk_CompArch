/* ECGR 5181 - Computer Architecture
 * Lab 02
 * Phillip Harmon
 * Master Event Queue Object Module */
#ifndef HARMON_MEQ
#define HARMON_MEQ

#include "Event.h"
#include <vector>
#include <queue>
#include <cstdlib>
#include <iostream>
using std::priority_queue;
using std::vector;
using std::cout;
using std::endl;

class MasterEventQueue{
		//MEQ implemented as a vector
		vector<Event> eventQueue;
	public:
		
		//constructor
		MasterEventQueue(void);
		MasterEventQueue(Event);

		//insert to front of meq
		void push_front(Event);

		//insert to back of meq
		void push_back(Event);

		//insert to point in meq
		void insert(int,Event);

		//get first element of meq
		Event top(void);
		
		//get and remove first element of meq
		Event pop(void);

		//get element at point in meq
		Event at(int);

		//get size of meq
		int size(void);

		//print the meq
		void print(void);
};

#endif
