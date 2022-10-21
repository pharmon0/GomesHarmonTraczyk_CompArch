/* ECGR 5181 Computer Architecture
 * Lab 03
 * Phillip Harmon 
 * Main Program Code */

//============================================
// Libraries
//============================================
#include "CPU.h"     //CPU Object Class

//============================================
// Main Program
//============================================
int main(void){
    //Instantiate CPU Object
    CPU my_processor;
    //Run CPU
    my_processor.run();


    return 0;
}