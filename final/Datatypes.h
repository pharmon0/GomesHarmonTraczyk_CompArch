/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Datatypes Module */

// Start Header Guard
#ifndef GHT_DATATYPE
#define GHT_DATATYPE

//============================================
// Headers and Namespaces
//============================================
#include <string>
#include <cstdint>
#include "MemoryBlock.h"
using std::string;

//============================================
// Datatypes
//============================================
//response_t used to carry results of timed controlled operations
typedef struct{
    uint32_t data;
    Block block;
    string reason;
    bool success;
} response_t;

// End Header Guard
#endif