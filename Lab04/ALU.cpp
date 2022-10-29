/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * ALU Function Code
 *
 * -----------------------------------------------------------------------------------------
 * C++ SRA Implementation inspired by:
 * https://stackoverflow.com/questions/53746160/how-to-implement-arithmetic-right-shift-in-c
 * Posted by user palotasb on 12/13/18
 * -----------------------------------------------------------------------------------------
 *
 */
#include "ALU.h"

//============================================
// Constructors
//============================================
ALU::ALU(void){}
ALU::ALU(uint8_t intTickCount, uint8_t flopTickCount){
    this->intTicks = intTickCount;
    this->flopTicks = flopTickCount;
    this->counter    = 0;
    this->A.uinteger = 0;
    this->B.uinteger = 0;
    this->X.uinteger = 0;
    this->aluctrl.all   = 0;
}

//============================================
// ALU::process
// - ticks the ALU
//============================================
void ALU::process(void){
    if(this->counter == 0){
        if(this->aluctrl.alufp){
            this->counter = this->flopTicks;
        }else{
            this->counter = this->intTicks;
        }
        this->aluctrl.busy = 1;
    }
    this->counter--;
    if(this->counter == 0){
        this->aluctrl.busy = 0;
        this->operate();
    }
}

//============================================
// ALU::operate
// - updates ALU Data by operation
//============================================
void ALU::operate(void){
    if(this->aluctrl.alufp){
        switch(this->aluctrl.aluop){
            case ALU_ADD :
                this->X.single = this->A.single + this->B.single;
            break;
            case ALU_SUB :
                this->X.single = this->A.single - this->B.single;
            break;
        }
    }else{
        switch(this->aluctrl.aluop){
            case ALU_ADD : this->X.integer = this->A.integer +  this->B.integer; break;
            case ALU_SUB : this->X.integer = this->A.integer -  this->B.integer; break;
            case ALU_AND : this->X.integer = this->A.integer &  this->B.integer; break;
            case ALU_OR  : this->X.integer = this->A.integer |  this->B.integer; break;
            case ALU_XOR : this->X.integer = this->A.integer ^  this->B.integer; break;
            case ALU_SLL : this->X.integer = this->A.integer << this->B.integer; break;
            case ALU_SRL : this->X.integer = this->A.integer >> this->B.integer; break;
            case ALU_SRA :
                //C++ SRA Implementation inspired by:
                //https://stackoverflow.com/questions/53746160/how-to-implement-arithmetic-right-shift-in-c
                //Posted by user palotasb on 12/13/18
                this->X.integer = (this->A.integer < 0)?
                                  (~((~this->A.integer)>>this->B.integer)):
                                  (this->A.integer>>this->B.integer);
            break;
            case ALU_CMP :
                switch(this->aluctrl.alucomp){ 
                    case 0b00: this->X.integer = (this->A.integer <  this->B.integer)?(1):(0); break;
                    case 0b01: this->X.integer = (this->A.integer == this->B.integer)?(1):(0); break;
                    case 0b10: this->X.integer = (this->A.integer != this->B.integer)?(1):(0); break;
                    case 0b11: this->X.integer = (this->A.integer >= this->B.integer)?(1):(0); break;
                }
            break;
            case ALU_CMPU:
                switch(this->aluctrl.alucomp){ 
                    case 0b00: this->X.integer = ((this->A.uinteger) <  (this->B.uinteger))?(1):(0); break;
                    case 0b01: this->X.integer = ((this->A.uinteger) == (this->B.uinteger))?(1):(0); break;
                    case 0b10: this->X.integer = ((this->A.uinteger) != (this->B.uinteger))?(1):(0); break;
                    case 0b11: this->X.integer = ((this->A.uinteger) >= (this->B.uinteger))?(1):(0); break;
                }
            break;
        }
    }
}