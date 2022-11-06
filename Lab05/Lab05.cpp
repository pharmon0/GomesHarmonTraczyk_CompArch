/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Main Program Code */

//============================================
// Libraries
//============================================
#include "Cache.h"
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
//#include <cstring>
#include "Functions.h"
using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stoul;
using std::tolower;

//============================================
// Constants and Definitions
//============================================
vector<uint32_t> readFile(string filename);

//============================================
// Main Program
//============================================
int main(void){
    vector<uint32_t> addresses = readFile("text/addresses.txt");

    //Cache cache1 = Cache(256, 32, CACHE_ASS_DIRECT);
    //Cache cache2 = Cache(512, 32, CACHE_ASS_DIRECT);
    //Cache cache3 = Cache(256, 64, CACHE_ASS_DIRECT);
    //Cache cache4 = Cache(256, 32, CACHE_ASS_4WAY);

    return 0;
}

//============================================
// Main Program
//============================================
vector<uint32_t> readFile(string filename){
    ifstream file;
    file.open(filename);
    if(!file.is_open()){
        cout << "FILE '" << filename << "' DID NOT OPEN" << endl;
        return {};
    }
    string value;
    vector<uint32_t> result;
    cout << " Reading File!" << endl;
    while( file >> value ){
        uint32_t item = uint32_t(stoul(value, nullptr, 16));
        result.push_back(item);
    }
    file.close();
    return result;
}
