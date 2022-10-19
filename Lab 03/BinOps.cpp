/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Binary Operations Function Code */
#include "BinOps.h"

//============================================
// hexString
// - converts a 32bit binary value into hex string
//============================================
string hexString(uint32_t bin){
    string res = "0x";
    for(int i = 28; i >= 0; i-=4){
        res += binToHex((bin>>i)&0xF);
    }
    return res;
}

//============================================
// binToHex
// - converts a binary value into hex
//============================================
char binToHex(uint8_t bin){
    char res;
    switch(bin&0xF){
        case 0: res = '0'; break;
        case 1: res = '1'; break;
        case 2: res = '2'; break;
        case 3: res = '3'; break;
        case 4: res = '4'; break;
        case 5: res = '5'; break;
        case 6: res = '6'; break;
        case 7: res = '7'; break;
        case 8: res = '8'; break;
        case 9: res = '9'; break;
        case 10: res = 'A'; break;
        case 11: res = 'B'; break;
        case 12: res = 'C'; break;
        case 13: res = 'D'; break;
        case 14: res = 'E'; break;
        case 15: res = 'F'; break;
        default: res = '?'; break;
    }
    return res;
}