/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pseudo-Core Module */
#include "Core.h"

Core::Core(Cache* mem){
    this->cache = mem;
}