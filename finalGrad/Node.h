/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Directory Node Module */

// Start Header Guard
#ifndef GHT_DIRECTORY_NODE
#define GHT_DIRECTORY_NODE

//==========================
// Headers and Namespaces
//==========================
#include <cstdint>
#include <string>
#include <vector>
#include "Settings.h"
#include "Memory.h"
#include "Cache.h"
using std::string;
using std::vector;

class Node{

    //Physical Components
    //Core* core;
    Cache* cache;
    Memory* memory;
    vector<Node*> connections;
    
    //Metadata
    string node_name;
    uint8_t node_id;

  public:

    //Constructor
    Node(string name, uint8_t id, Cache* cache_pointer, Memory* memory_pointer);

    //Node Setup Functions
    void add_connection(Node* node_pointer);

};

// End Header Guard
#endif