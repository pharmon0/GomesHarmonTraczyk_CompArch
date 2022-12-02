/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Definitions Module */

// Start Header Guard
#ifndef GHT_HELPER_HEADER
#define GHT_HELPER_HEADER

//============================================
// Headers and Namespaces
//============================================
#include <string>
#include <cstdint>
using std::string;

//============================================
// Constants and Macros
//============================================
#define SYSTEM_BITWIDTH 32

//============================================
// Datatypes
//============================================
//response_t used to indicate completed results
typedef struct{
    uint32_t data;
    string reason;
    bool success;
} response_t;

//End Header Guard
#endif