/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Machine Code Assembler */

//============================================
// Libraries
//============================================
#include "RISCV.h"   //RISCV Definitions Header
#include <iostream>  //basic console i/o
#include <cstdint>   //Standard Int Types
#include <fstream>   //file i/o
#include <string>    //string datatype
using std::string;
using std::fstream;
using std::cout;
using std::endl;
using std::stoi;

//============================================
// Constants and Definitions
//============================================

//============================================
// Function Prototypes
//============================================
bool isInteger(char);
uint32_t  vRs1(uint32_t);
uint32_t  vRs2(uint32_t);
uint32_t   vRd(uint32_t);
uint32_t vImmI(uint32_t);
uint32_t vImmS(uint32_t);
uint32_t vImmB(uint32_t);
uint32_t vImmU(uint32_t);
uint32_t vImmJ(uint32_t);

//============================================
// Main Function
//============================================
int main(int argc, char *argv[]){
    //parse input arguments
    if(argc != 3) return 1;
    string filenameIn(argv[1]);
    string filenameOut(argv[2]);
    
    //prepare and open files
    fstream fileOut;
    fstream fileIn;
    fileOut.open(filenameOut, fstream::out | fstream::trunc);
    if(!fileOut.is_open()) return 1;
    fileIn.open(filenameIn, fstream::in);
    if(!fileIn.is_open()) return 1;
    
    //Assembler Operation goes here
    string field;
    while(fileIn >> field){
        cout << field;
        if(field.at(0) == '#'){
            //item is a comment.
            getline(fileIn,field,'\n');
            cout << endl;
            continue;
        }else if(field.at(0) == 'x'){
            //item is an integer register
            field.erase(field.begin());
            if(isInteger(field.at(0))){
                //item is numeric int register
                cout << " :: Integer Register : " << stoi(field);
            } else{
                //item is named int register
                cout << " :: Named Integer Register : " << field;
            }
        }else if(field.at(0) == 'f'){
            //item is a float register
            field.erase(field.begin());
            cout << " :: Float Register : " << stoi(field);
        }else if(isInteger(field.at(0))){
            //item is an immediate
            cout << " :: Immediate : " << stoi(field);
        }else{

        }
        cout << endl;
    }
    

    //close files
    fileOut.close();
    fileIn.close();

    return 0;
}

//============================================
// Helper Functions
//============================================
bool isInteger(char c){
    if(  c == '0'
      || c == '1'
      || c == '2'
      || c == '3'
      || c == '4'
      || c == '5'
      || c == '6'
      || c == '7'
      || c == '8'
      || c == '9' )
        return true;
    return false;
}
uint32_t vImmI(uint32_t imm){
    //imm is [11:0]
    return (imm & 0x00000FFF) << SHLMT_Iim11_0;
}
uint32_t vImmS(uint32_t imm){
    //imm is [11:0]
    uint32_t res = 0x00000000;
    res |= (imm & 0x00000FE0) << SHLMT_Sim11_5;
    res |= (imm & 0x0000001F) << SHLMT_Sim4_0;
    return res;
}
uint32_t vImmB(uint32_t imm){
    //imm is [12:1]
    uint32_t res = 0x00000000;
    res |= (imm & 0x00001000) << SHLMT_Bim12;
    res |= (imm & 0x00000800) >> SHRMT_Bim11;
    res |= (imm & 0x000007E0) << SHLMT_Bim10_5;
    res |= (imm & 0x0000001E) << SHLMT_Bim4_1;
    return res;
}
uint32_t vImmU(uint32_t imm){
    //imm is [31:12]
    return imm & 0xFFFFF000;
    //No Shift Required
}
uint32_t vImmJ(uint32_t imm){
    //imm is [20:1]
    uint32_t res = 0x00000000;
    res |= (imm & 0x00100000) << SHLMT_Jim20;
    res |= (imm & 0x000FF000) << SHLMT_Jim19_12;
    res |= (imm & 0x00000800) << SHLMT_Jim11;
    res |= (imm & 0x000007FE) << SHLMT_Jim10_1;
    return res;
}
uint32_t vRs1(uint32_t val){return ((val&0x1F) << SHLMT_rs1);}
uint32_t vRs2(uint32_t val){return ((val&0x1F) << SHLMT_rs2);}
uint32_t  vRd(uint32_t val){return ((val&0x1F) << SHLMT_rd) ;}