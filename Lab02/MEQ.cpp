/* ECGR 5181 - Computer Architecture
 * Lab 02
 * Phillip Harmon
 * Definition Code for MEQ.h */
#include "MEQ.h"

//insert element to front of meq
void MasterEventQueue::push_front(Event e){
	this->eventQueue.insert(this->eventQueue.begin(), e);
}

//insert element to back of meq
void MasterEventQueue::push_back(Event e){
	this->eventQueue.push_back(e);
}

//insert element to point in meq
void MasterEventQueue::insert(int pos,Event e){
	this->eventQueue.insert(this->eventQueue.begin()+pos,e);
}

//get top element of meq
Event MasterEventQueue::top(void){
	return this->eventQueue.at(0);
}

//get and remove top element of meq
Event MasterEventQueue::pop(void){
	Event e = this->eventQueue.at(0);
	this->eventQueue.erase(this->eventQueue.begin());
	return e;
}

//get element at a point in meq
Event MasterEventQueue::at(int pos){
	return this->eventQueue.at(pos);
}

//get the size of the meq
int MasterEventQueue::size(void){
	return this->eventQueue.size();
}

//print the meq element-by-element
void MasterEventQueue::print(void){
	int size = this->eventQueue.size();
	for(int i = 0; i < size; i++){
		if(this->eventQueue.at(i).isnt("init")){
			cout << ">\t";
			eventQueue.at(i).print();
			cout << endl;
		}
	}
}

//constructor
MasterEventQueue::MasterEventQueue(void){
	this->eventQueue.insert(this->eventQueue.begin(),Event("{noop init3}",0,'F'));
	this->eventQueue.insert(this->eventQueue.begin(),Event("{noop init2}",0,'D'));
	this->eventQueue.insert(this->eventQueue.begin(),Event("{noop init1}",0,'E'));
}

//constructor
MasterEventQueue::MasterEventQueue(Event e){
	this->eventQueue.insert(this->eventQueue.begin(),e);
	this->eventQueue.insert(this->eventQueue.begin(),Event("{noop init2}",0,'D'));
	this->eventQueue.insert(this->eventQueue.begin(),Event("{noop init1}",0,'E'));
}
