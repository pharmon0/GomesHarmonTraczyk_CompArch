/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Definitions Module */

// Start Header Guard
#ifndef GHT_HELPER_HEADER
#define GHT_HELPER_HEADER

//============================================
// Constants and Macros
//============================================
#define SYSTEM_BITWIDTH 32
#define MESI_I 'I'
#define MESI_E 'E'
#define MESI_S 'S'
#define MESI_M 'M'
#define BUS_INVALIDATE "invalidate"
#define BUS_RWITM "read_and_invalidate"
#define BUS_READ "read"
#define BUS_DUMP "dump"
#define BUS_SAVE "save"

//============================================
// Class Prototypes
//============================================
class Cache;
class Core;
class Bus;
class Block;
class Memory;

//End Header Guard
#endif