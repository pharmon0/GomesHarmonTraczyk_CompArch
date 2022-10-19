/* ECGR 5181 Computer Architecture
 * Lab 02
 * Phillip Harmon 
 * Main Program Code */
//=====================================================================
// Inclusions
//=====================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include "Event.h"
#include "MEQ.h"
#include <vector>
#include <queue>
using std::cout;
using std::endl;
using std::priority_queue;
using std::vector;
using std::string;
using std::setw;
using std::setfill;
//=====================================================================
// Constants and Defines
//=====================================================================
//instruction queue
const string IQ[] = {
	"{LOOP: fld f0,0(x1)}",
	"{fadd.d f4,f0,f2}",
	"{fsd f4,0(x1)}",
	"{addi x1,x1,-8}",
	"{bne x1,x2,LOOP}",
	"{halt}",
	"{noop halt0}",
	"{noop halt1}",
	"{noop halt2}"
};
#define DEBUG_MODE
//=====================================================================
// Main Function
//=====================================================================
int main(void){
	#ifndef DEBUG_MODE
	cout << setfill('-') << setw(160) << "" << endl;
	#endif

	//Program Counter
	int PC = 0;
	
	//system Clock incrementer
	int CLK = 0;
	
	//Iteration Register
	int x1 = 20, x2 = 0;
	
	//register to store jump-back PC address on failed branch predict
	int PCBACKUP = 0;
	
	//MEQ
	MasterEventQueue meq = MasterEventQueue();

	//Control Flags
	//Flag to prevent update of program counter
	bool updatePC = true;
	//Flag to insert a stall
	bool stall = false;
	//Flag to insert two stalls
	bool doubleStall = false;
	//Flag to clear pipeline for branching
	bool clearPipeline = false;
	//Flag to take loop address
	bool takeLoop = false;
	//Flag to restore PC in case of bad branch
	bool restorePC = false;
	//stall processor
	bool stalling = false;
	
	//loop forever (until halt)
	while(1){ // clock cycle loop
	
		//------------------
		//Debug print meq
		//------------------
		#ifdef DEBUG_MODE
		cout << "\n=====Clock Cycle " << CLK << "====="
			<< "\n> x1 : " << x1 << " x2 : " << x2
			<< "\n> PC : " << PC
			<< "\n> MEQ : " 
			<< endl;
		meq.print();
		cout<<"---------------"<<endl;
		#endif

		//------------------
		//Reset flags
		//------------------
		updatePC = true;
		stall = false;
		doubleStall = false;
		clearPipeline = false;
		takeLoop = false;
		restorePC = false;
		stalling = false;
		
		//------------------
		//load pipeline regs
		//------------------
		Event E = meq.pop();
		Event D = meq.pop();
		Event F = meq.pop();
		Event fetch = Event();
		if(meq.size() == 0){
			//fetch from instruction memory
			fetch = Event(IQ[PC],CLK);
		} else{
			//fetch from event
			fetch = meq.pop();
		}
		
		//------------------
		//Debug Print Pipeline Registers
		//------------------
		#ifdef DEBUG_MODE
		cout << "Pipeline Stages:";
		if(E.isnt("init")){
			cout << "\n\twriteback : ";
			E.print();
		}
		if(D.isnt("init")){
			cout << "\n\texecute : ";
			D.print();
		}
		if(F.isnt("init")){
			cout << "\n\tdecode : ";
			F.print();
		}
		cout << "\n\tfetch : ";
		fetch.print();
		cout << "\n---------------" << endl;
		#endif

		//------------------
		//Program Execution Printing
		//------------------
		#ifndef DEBUG_MODE
		cout << setfill(' ')
			<< "Clock Cycle #" << CLK
			<< setw(15-std::to_string(CLK).size()) << "fetch : " << fetch.inst;
		if(F.isnt("init"))
			cout << setw(35-fetch.inst.size()) << "decode : " << F.inst;
		if(D.isnt("init"))
			cout << setw(35-F.inst.size()) << "execute : " << D.inst;
		if(E.isnt("init"))
			cout << setw(35-D.inst.size()) << "writeback : " << E.inst;
		cout << "\n" << setfill('-') << setw(160) << "" << endl;
		#endif

		//------------------
		//Control decoding
		//------------------
		//if stall in E
		if(E.is("stall")){
			#ifdef DEBUG_MODE
			cout << "DEBUG: stall[E], Stalling" << endl;
			#endif
			//stall
			stalling = true;

			if(D.is("stall")){
				//do nout update PC
				updatePC = false;
			}

			//if bne in fetch
			if(fetch.is("bne")){
				#ifdef DEBUG_MODE
				cout << "DEBUG: stalling but found bne[F], Predicting Branch" << endl;
				#endif
				//assume branch will be taken (assume x1 != x2)
				//do not update PC
				updatePC = false;
				//take loop
				takeLoop = true;
			}
		}
		
		//stall CPU
		if(!stalling){
			#ifdef DEBUG_MODE
			cout << "DEBUG: CPU NOT STALLED" << endl;
			#endif
			//if fld in E and fadd in D
			if(E.is("fld") && D.is("fadd")){
				#ifdef DEBUG_MODE
				cout << "DEBUG: fld[E]->fadd[D], Inserting Stall" << endl;
				#endif
				//insert stall into E position
				stall = true;
				//do not update PC
				updatePC = false;
			}
			
			//if fadd in E and fsd in D
			if(E.is("fadd") && D.is("fsd")){
				#ifdef DEBUG_MODE
				cout << "DEBUG: fadd[E]->fsd[D], Inserting Stalls" << endl;
				#endif
				//insert stall into E and D position
				stall = true;
				doubleStall = true;
				//do not update PC
				updatePC = false;
			}
			
			//if bne in fetch
			if(fetch.is("bne")){
				#ifdef DEBUG_MODE
				cout << "DEBUG: bne[F], Predicting Branch" << endl;
				#endif
				//assume branch will be taken (assume x1 != x2)
				//do not update PC
				updatePC = false;
				//take loop
				takeLoop = true;
			}
			
			//if bne in E and x1 == x2
			if(E.is("bne") && (x1 == x2)){
				#ifdef DEBUG_MODE
				cout << "DEBUG: bne[E] and x1==x2, Undoing Prediction" << endl;
				#endif
				//branch prediction failed.
				//restore PC
				restorePC = true;
				//clear stages F and D
				clearPipeline = true;
			}
			
			//------------------
			//Program Execution
			//------------------
			//if halt in D
			if(D.is("halt")){
				#ifdef DEBUG_MODE
				cout << "DEBUG: halt[D] executing ..." << endl;
				#endif
				//end program
				cout << "\n >> PROGRAM END (halt instruction trigger)\n" << endl;
				return 0;
			}
			
			//if addi in E
			if(E.is("addi")){
				#ifdef DEBUG_MODE
				cout << "DEBUG: addi[E] executing, Decrementing x1" << endl;
				#endif
				//decrement x1
				x1 -= 4;
			}

		}

		//------------------
		//update program counter
		//------------------
		if(restorePC){
			#ifdef DEBUG_MODE
			cout << "DEBUG: restorePC triggered" << endl;
			#endif
			//restore PC address
			PC = PCBACKUP;
		}
		if(takeLoop){
			#ifdef DEBUG_MODE
			cout << "DEBUG: takeLoop triggered" << endl;
			#endif
			if(!stall){
				#ifdef DEBUG_MODE
				cout << "\tPC branching..." << endl;
				#endif
				//store previous PC
				PCBACKUP = PC;
				//set PC to loop start
				PC = 0;
			}
			#ifdef DEBUG_MODE
			else cout << "\tstalling takeLoop. PC not updated." << endl;
			#endif
		}
		if(updatePC){
			#ifdef DEBUG_MODE
			cout << "DEBUG: updatePC triggered. PC++" << endl;
			#endif
			//PC = PC+1
			PC++;
		}
		
		//------------------
		//repopulate the meq
		//apply stalls
		//perform fetch
		//------------------
		if(stalling && D.is("stall")){
			#ifdef DEBUG_MODE
			cout << "DEBUG: Double Stall MEQ Handler triggered" << endl;
			#endif
			meq.push_front(fetch);
			meq.push_front(F);
			meq.push_front(D);
		} else if(stall){
			#ifdef DEBUG_MODE
			cout << "DEBUG: stall MEQ Handler triggered. Inserting stall" << endl;
			#endif
			//hold F
			meq.push_front(F);
			//hold D
			meq.push_front(D);
			//insert stall E
			meq.push_front(Event("{noop stall}",CLK+1,'E'));
			//double stall?
			if(doubleStall){
				#ifdef DEBUG_MODE
				cout << "\tInserting Second stall" << endl;
				#endif
				meq.insert(1,Event("{noop stall}",CLK+1,'D'));
			}
		} else if(restorePC){
			#ifdef DEBUG_MODE
			cout << "DEBUG: Branch Failed. Clearing Pipeline" << endl;
			#endif
			//clear MEQ / flush pipeline
			while(meq.size() != 0){meq.pop();}
			meq.push_front(Event("{noop branch}",CLK+1,'F'));
			meq.push_front(Event("{noop branch}",CLK+1,'D'));
			meq.push_front(Event("{noop branch}",CLK+1,'E'));
		} else{
			#ifdef DEBUG_MODE
			cout << "DEBUG: Standard MEQ Handler triggered. Advancing MEQ" << endl;
			#endif
			//advance fetch to F
			meq.push_front(Event(fetch.inst,CLK+1,'F'));
			//advance F to D
			meq.push_front(Event(F.inst,CLK+1,'D'));
			//advance D to E
			meq.push_front(Event(D.inst,CLK+1,'E'));
			//E advances to W aka delete
		}

		//------------------
		//DEBUG Flag Printout
		//------------------
		#ifdef DEBUG_MODE
		cout << "> Flags:\n> updatePC\tstall\tdoubleStall\tclearPipeline\ttakeLoop\trestorePC\tstalling\n  "
			<< updatePC << "\t\t" << stall << "\t" << doubleStall << "\t\t" << clearPipeline
			<< "\t\t" << takeLoop << "\t\t" << restorePC << "\t\t" << stalling << endl;

		//------------------
		//DEBUG PROGRAM HALT
		//------------------
		if(CLK > 500){
			cout << "DEBUG: MAX RUNTIME. ENDING..." << endl;
			return 1;
		}
		#endif
		
		//------------------
		//increment system clock
		//------------------
		CLK++;
	}

	return 1; //return 1. Program should never get here
}
