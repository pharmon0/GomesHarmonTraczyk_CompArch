/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Directory Node Module */
#include "Node.h"

//================================
// Node Constructor
//================================
Node::Node(string name, uint8_t id, Cache* cache_pointer, Memory* memory_pointer){
    this->node_name = name;
    this->node_id = id;
    this->cache = cache_pointer;
    this->memory = memory_pointer;
}

//================================
// Process a tick in the node
//================================
void Node::process(){
    //TODO Node::process()
}

//================================
// Add an interconnect to the node
//================================
void Node::add_connection(Node* node_pointer){
    this->connections.push_back(node_pointer);
}

