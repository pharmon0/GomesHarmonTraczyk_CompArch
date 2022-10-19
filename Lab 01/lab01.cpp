/* ECGR 5181 Computer Architecture
 * 	Lab 01
 * 	Phillip Harmon */
//=====================================================================
// Inclusions
//=====================================================================
#include <iostream>
#include <queue>
#include <vector>
#include <chrono>
using std::cout; using std::endl;
using std::priority_queue;
using std::vector;
//=====================================================================
// Global Constants
//=====================================================================
#define INITIAL_EVENTS 20U //population size of event queue
#define EVENT_COUNT 100U //number of events to process
//=====================================================================
// Function Prototypes
//=====================================================================
unsigned long getTimeMS(void); //returns epoch time in ms
//=====================================================================
// Class Definitions
//=====================================================================
//event item for populating the event queue
class Event{
  public:
	unsigned long t; //execution time
	int v;//value
	
	//construct a blank-slate, new event
	Event(void){
		this->t = getTimeMS();
		this->v = rand();
	}
	//execute an event
	Event execute(void){
		std::srand(this->v);
		Event result = Event();
		result.t = this->t+rand();
		result.v = this->v;
		return result;
	}
};
//=====================================================================
// Main Function
//=====================================================================
int main(void){
	//seed the RNG
	std::srand(getTimeMS());
	
	//produce the event queue
	struct EventQueueSorting{
		bool operator() (Event lhs, Event rhs){return lhs.t>rhs.t;}
	};
	priority_queue<Event,vector<Event>,EventQueueSorting> eventQueue //main queue
							     ,printQ;    //printing queue
	
	//initially populate the event queue
	cout<<"Initially Populating Queue"<<endl;
	for(unsigned i = 0; i < INITIAL_EVENTS; i++){
		Event event = Event();
		eventQueue.push(event);
		
		//Print the new item
		// cout<<"i="<<i<<" t="<<event.t<<" v="<<event.v<<endl;
	}
	
	//print the populated, sorted event queue
	printQ = eventQueue;
	unsigned len = printQ.size();
	cout<<"\nInitial Sorted Queue | size="<<len<<endl;
	for(unsigned j = 0; j < len; j++){
		Event e = printQ.top();
		printQ.pop();
		cout<<"    t="<<e.t<<" v="<<e.v<<endl;
	}
	
	//run the event queue
	cout<<"\nRunning Queue\n  Note: starts @ 0, so final is 99."<<endl;
	for(unsigned i = 0; i < EVENT_COUNT; i++){
		cout<<"\n Event Number "<<i<<endl;
		//pop the next event E(t,v)
		Event oldE = eventQueue.top();
		eventQueue.pop();
		cout<<"  Popped Old Event:"<<"t="<<oldE.t<<" v="<<oldE.v<<endl;
		//generate a new event E'(t',v): t'=t+irand(v)
		Event newE = oldE.execute();
		cout<<"  Pushed New Event:"<<"t="<<newE.t<<" v="<<newE.v<<endl;
		eventQueue.push(newE);
		
		//print the event queue
		// printQ = eventQueue;
		// unsigned len = printQ.size();
		// cout<<" Queue Overview | size="<<len<<endl;
		// for(unsigned j = 0; j < len; j++){
		// 	Event e = printQ.top();
		// 	printQ.pop();
		// 	cout<<"    t="<<e.t<<" v="<<e.v<<endl;
		// }
	}
	
	//print the final event queue state
	printQ = eventQueue;
	len = printQ.size();
	cout<<"\nFinal Event Queue | size="<<len<<endl;
	for(unsigned j = 0; j < len; j++){
		Event e = printQ.top();
		printQ.pop();
		cout<<"    t="<<e.t<<" v="<<e.v<<endl;
	}
	
	return 0; //exit code 0
}
//=====================================================================
// Function returns epoch time in ms using chrono
//=====================================================================
unsigned long getTimeMS(void){
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
}
