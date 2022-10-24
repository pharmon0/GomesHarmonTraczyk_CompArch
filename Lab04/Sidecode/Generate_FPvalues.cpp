/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Populate file with random FP values */

//============================================
// Libraries
//============================================
#include <iostream>  //basic console i/o
#include <cstdint>   //Standard Int Types
#include <fstream>   //file i/o
#include <string>    //string datatype
#include <ctime>     //time for seeding
#include <stdlib.h>
using std::string;
using std::fstream;
using std::cout;
using std::endl;
using std::stoi;

//============================================
// Main Function
//============================================
int main(){
    //Populating txt file with random floating point values between LO and HI
    std::ofstream outputFile;
    outputFile.open("floatValues.txt");
    srand(time(NULL));
    float LO = -100;
    float HI = 100;
    for (int i = 0; i < 256; i++) {
        float r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        outputFile << r << endl;
    }

    //Test population of array from the previous txt file
    std::ifstream inputFile;
    inputFile.open("floatValues.txt");
    float arrayTest[256];
    for (int i = 0; i < 256; i++) {
        inputFile >> arrayTest[i];
    }
    
    //Testing population of array
    for (int i = 0; i < 256; i++) {
        cout << arrayTest[i] << endl;
    }

    
    return 0;
}