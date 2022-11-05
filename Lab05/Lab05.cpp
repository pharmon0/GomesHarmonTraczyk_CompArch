/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Main Program Code */

//============================================
// Libraries
//============================================
#include "Cache.h"

//============================================
// Constants and Definitions
//============================================

//============================================
// Main Program
//============================================
int main(void){
    Cache cache1 = Cache(256, 32, CACHE_ASS_DIRECT);
    Cache cache2 = Cache(512, 32, CACHE_ASS_DIRECT);
    Cache cache3 = Cache(256, 64, CACHE_ASS_DIRECT);
    Cache cache4 = Cache(256, 32, CACHE_ASS_4WAY);

    return 0;
}