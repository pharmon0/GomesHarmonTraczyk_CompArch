/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Sign Extender Function Code */
#include "SignExtend.h"

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