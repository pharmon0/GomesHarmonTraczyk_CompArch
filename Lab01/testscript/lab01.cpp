/* ECGR 5181 Computer Architecture
 * 	Lab 01
 * 	Phillip Harmon */

//inclusions
#include <iostream>
#include <queue>
#include <vector>
#include <chrono>

//prototypes

//class definitions
class Event{
  public:
	unsigned long t;
	int v;
	
	Event(unsigned long seed){
		this->t = seed + (rand()%100);
		this->v = rand();
	}
};

struct EventQueueSorting{
	bool operator() (Event lhs, Event rhs){return lhs.t>rhs.t;}
};

//main function
int main(void){
	unsigned long seedTime = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	
	std::srand(seedTime);
	
	std::priority_queue<Event,std::vector<Event>,EventQueueSorting> eventQueue;
	
	std::cout<<"Populating Queue"<<std::endl;
	for(int i = 0; i < 10; i++){
		Event event = Event(seedTime);
		eventQueue.push(event);
		std::cout<<"i="<<i<<" t="<<event.t<<" v="<<event.v<<std::endl;
	}
	std::cout<<"Printing Ordered Queue"<<std::endl;
	int queueSize = eventQueue.size();
	for(int i = 0; i < queueSize; i++){
		Event event = eventQueue.top();
		std::cout<<"i="<<i<<" t="<<event.t<<" v="<<event.v<<std::endl;
		eventQueue.pop();
	}
	std::cout<<"items remaining in queue: "<<eventQueue.size()<<std::endl;
	return 0;
}
