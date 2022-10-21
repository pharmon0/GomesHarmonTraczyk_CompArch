/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Helper Function Code */
#include "Functions.h"

//============================================
// hexString
// - converts a 32bit binary value into hex string
//============================================
string hexString(uint32_t bin){
    string res = "0x";
    for(uint8_t i = 28; i >= 0; i-=4){
        //iterate through bin, 4b at a time
        switch((bin>>i)&0xF){
            case  0: res += '0'; break;
            case  1: res += '1'; break;
            case  2: res += '2'; break;
            case  3: res += '3'; break;
            case  4: res += '4'; break;
            case  5: res += '5'; break;
            case  6: res += '6'; break;
            case  7: res += '7'; break;
            case  8: res += '8'; break;
            case  9: res += '9'; break;
            case 10: res += 'A'; break;
            case 11: res += 'B'; break;
            case 12: res += 'C'; break;
            case 13: res += 'D'; break;
            case 14: res += 'E'; break;
            case 15: res += 'F'; break;
            default: res += '?'; break;
        }
    }
    return res;
}

//============================================
// memSignExtend
// - returns sign-extended memory load value
//============================================
//                     memdataout    memrsgn       memrsz
int32_t memSignExtend(int32_t input, bool enable, uint8_t size){
    if(!enable) return input;
    switch(size){
        case 0b01: //Size == 1B
            input |= (input&0x80)?(0xFFFFFF00):(0);
        break;
        case 0b10: //Size == 1H (2B)
            input |= (input&0x8000)?(0xFFFF0000):(0);
        break;
    }
    return input;
}

//============================================
// immSignExtendShift
// - returns sign-extended immediate value, shifted into correct bit position
//============================================
//                             immval       imsize       imshft
int32_t immSignExtendShift(int32_t input, bool size, bool shift){
    if(size){ //sign-extend. 12-bit
        input |= (input&0x800)?(0xFFFFF000):(0);
    }else{
        input |= (input&0x80000)?(0xFFF00000):(0);
    }
    if(size != shift) //size XOR shift -> shift by 1
        return input << 1;
    if(size && shift) //size AND shift -> shift by 12
        return input << 12;
    return input;     //!size AND !shift -> no shift
}