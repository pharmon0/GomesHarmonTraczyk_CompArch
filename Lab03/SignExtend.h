/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Sign Extender Module */

// Start Header Guard
#ifndef HARMON_SIGNEXTEND
#define HARMON_SIGNEXTEND

//============================================
// Libraries
//============================================
#include <string>    //String Datatype
using std::string;
using std::to_string;

//============================================
// Constants and Definitions
//============================================

//============================================
// Function Prototypes
//============================================
int32_t memSignExtend(int32_t,bool,uint8_t);
int32_t immSignExtendShift(int32_t,bool,bool);

// End Header Guard
#endif