/* ECGR 5181 - Computer Architecture
 * Lab 02
 * Phillip Harmon
 * Event Object Module */
#ifndef HARMON_EVENT
#define HARMON_EVENT

#include <string>
#include <iostream>
using std::string;
using std::cout;

class Event{
	public:
	  	//assembly text
		string inst;
		//clock cycle #
		int cycle;
		//CPU pipeline stage (_, F, D, E)
		char stage;
		
		//constructors
		Event(void);
		Event(string, int);
		Event(string, int, char);
		
		//print inst[stage].cycle
		void print(void);

		//check if this->inst contains a string
		bool is(string);

		//check if this->inst does not contain a string
		bool isnt(string);
};	

#endif
