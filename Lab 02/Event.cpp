/* ECGR 5181 - Computer Architecture
 * Lab 02
 * Phillip Harmon
 * Definition Code for Event.h */
#include "Event.h"

//constructor
Event::Event(void){
	this->inst = "nullInstr";
	this->cycle = -1;
	this->stage = '?';
}

//constructor
Event::Event(string instr, int cc){
	this->inst = instr;
	this->cycle = cc;
	this->stage = '_';
}

//constructor
Event::Event(string instr, int cc, char s){
	this->inst = instr;
	this->cycle = cc;
	this->stage = s;
}

//function prints the event as inst[stage].cycle
void Event::print(void){
	cout << this->inst;
	//hide stage for stall instructions
	if(this->isnt("stall"))
		cout << '(' << this->stage << ')';
	cout << '.' << this->cycle;
}

//check if this->inst contains s
bool Event::is(string s){
	return (this->inst.find(s) != -1);
}

//check if this->inst does not contain s
bool Event::isnt(string s){
	return (this->inst.find(s) == -1);
}
