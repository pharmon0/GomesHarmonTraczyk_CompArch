/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Main Program Code */

//============================================
// Libraries
//============================================
#include "Membus.h"
#include <iostream>
#include <cstdint>
#include <bitset>
#include <string>
using std::string;
using std::cout;
using std::endl;
using std::bitset;

//============================================
// Main Program
//============================================
int main(void){
    Membus system;
    system.ram.populate("A.txt",0x400); //populate A array
    system.ram.populate("B.txt",0x800); //populate B array
    system.ram.populate("asm.txt",0);   //populate instructions
    system.ram.printToFile("ramReadout");
    //do{}while(system.process());
    //long int clock = system.cpu.clock;
    //long int instructions = system.cpu.instcnt;
    //int cpi = clock / instructions;
    //int ticks = clock * 10;
    //cout << "With a total of " << clock
    //     << " Clock Cycles and " << instructions
    //     <<  " Instructions, the average system CPI was "
    //     << cpi
    //     << "\n\n Total System Ticks: " << ticks 
    //     << endl;

    return 0;
}