/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Helper Functions Module */

// Start Header Guard
#ifndef HARMON_FUNCTIONS
#define HARMON_FUNCTIONS

//============================================
// Libraries
//============================================
#include <cstdint>   //Standard Int Datatypes
#include <string>    //String Datatype
using std::string;

//============================================
// Constants and Definitions
//============================================

//============================================
// Function Prototypes
//============================================
string hexString(uint32_t);
int32_t memSignExtend(int32_t,bool,uint8_t);
int32_t immSignExtendShift(int32_t,bool,bool);

// End Header Guard
#endif