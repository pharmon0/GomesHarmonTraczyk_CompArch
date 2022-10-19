/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Binary Operations Module */

// Start Header Guard
#ifndef HARMON_BINOPS
#define HARMON_BINOPS

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
string bitString(uint32_t);
string hexString(uint32_t);
char binToHex(uint8_t bin);

// End Header Guard
#endif