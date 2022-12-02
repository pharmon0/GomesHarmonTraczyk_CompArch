/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Directory Node Module */
#include "Node.h"

Node::Node(){}

//================================
// Attach the cache to the node
//================================
void Node::attach_cache(Cache* cache_pointer){
    this->cache = cache_pointer;
}

//================================
// Attach the memory to the node
//================================
void Node::attach_memory(Memory* memory_pointer){
    this->memory = memory_pointer;
}

//================================
// Add an interconnect to the node
//================================
void Node::add_connection(Node* node_pointer){
    this->connections.push_back(node_pointer);
}

