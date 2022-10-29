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
ALU::ALU(uint8_t intClockCount, uint8_t flopClockCount){
    this->intCycles   = intClockCount;
    this->flopCycles  = flopClockCount;
    this->counter     = 0;
    this->A.uinteger  = 0;
    this->B.uinteger  = 0;
    this->X.uinteger  = 0;
    this->aluctrl.all = 0;
}

//============================================
// ALU::process
// - ticks the ALU
//============================================
void ALU::process(){
    if(this->aluctrl.alufp){
        //handle flops
        if(this->flopCycles > 1){
            if(this->aluctrl.busy && (this->counter == 0)){
                //busy but now done
                this->aluctrl.busy = 0;
                this->operate();
            }else if(this->aluctrl.busy){
                //busy and waiting
                this->counter--;
            }else{
                //not started yet
                this->aluctrl.busy = 1;
                this->counter = this->flopCycles;
            } 
        }else{
            //no timing needed
            this->aluctrl.busy = 0;
            this->operate();
        }
    }else{
        //handle intops
        if(this->intCycles > 1){
            if(this->aluctrl.busy && (this->counter == 0)){
                //busy but now done
                this->aluctrl.busy = 0;
                this->operate();
            }else if(this->aluctrl.busy){
                //busy and waiting
                this->counter--;
            }else{
                //not started yet
                this->aluctrl.busy = 1;
                this->counter = this->intCycles;
            } 
        }else{
            //no timing needed
            this->aluctrl.busy = 0;
            this->operate();
        }
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
                cout << " ALU: FADD" << endl;
            break;
            case ALU_SUB :
                this->X.single = this->A.single - this->B.single;
                cout << " ALU: FSUB" << endl;
            break;
        }
    }else{
        switch(this->aluctrl.aluop){
            case ALU_ADD :
                this->X.integer = this->A.integer +  this->B.integer;
                cout << " ALU: ADD" << endl;
            break;
            case ALU_SUB :
                this->X.integer = this->A.integer -  this->B.integer;
                cout << " ALU: SUB" << endl;
            break;
            case ALU_AND : 
                this->X.integer = this->A.integer &  this->B.integer;
                cout << " ALU: AND" << endl;
            break;
            case ALU_OR  : 
                this->X.integer = this->A.integer |  this->B.integer;
                cout << " ALU: OR" << endl;
            break;
            case ALU_XOR : 
                this->X.integer = this->A.integer ^  this->B.integer;
                cout << " ALU: XOR" << endl;
            break;
            case ALU_SLL : 
                this->X.integer = this->A.integer << this->B.integer;
                cout << " ALU: SLL" << endl;
            break;
            case ALU_SRL : 
                this->X.integer = this->A.integer >> this->B.integer;
                cout << " ALU: SRL" << endl;
            break;
            case ALU_SRA :
                //C++ SRA Implementation inspired by:
                //https://stackoverflow.com/questions/53746160/how-to-implement-arithmetic-right-shift-in-c
                //Posted by user palotasb on 12/13/18
                this->X.integer = (this->A.integer < 0)?
                                  (~((~this->A.integer)>>this->B.integer)):
                                  (this->A.integer>>this->B.integer);
                cout << " ALU: SRA" << endl;
            break;
            case ALU_CMP :
                cout << " ALU: COMPARE: ";
                switch(this->aluctrl.alucomp){ 
                    case 0b00: this->X.integer = (this->A.integer <  this->B.integer)?(1):(0); cout << "<" << endl; break;
                    case 0b01: this->X.integer = (this->A.integer == this->B.integer)?(1):(0); cout << "==" << endl; break;
                    case 0b10: this->X.integer = (this->A.integer != this->B.integer)?(1):(0); cout << "!=" << endl; break;
                    case 0b11: this->X.integer = (this->A.integer >= this->B.integer)?(1):(0); cout << ">=" << endl; break;
                }
            break;
            case ALU_CMPU:
                cout << " ALU: COMPARE UNSIGNED: ";
                switch(this->aluctrl.alucomp){ 
                    case 0b00: this->X.integer = ((this->A.uinteger) <  (this->B.uinteger))?(1):(0); cout << "<" << endl; break;
                    case 0b01: this->X.integer = ((this->A.uinteger) == (this->B.uinteger))?(1):(0); cout << "==" << endl; break;
                    case 0b10: this->X.integer = ((this->A.uinteger) != (this->B.uinteger))?(1):(0); cout << "!=" << endl; break;
                    case 0b11: this->X.integer = ((this->A.uinteger) >= (this->B.uinteger))?(1):(0); cout << ">=" << endl; break;
                }
            break;
        }
    }
}