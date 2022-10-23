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
ArithmaticLogicUnit::ArithmaticLogicUnit(void){
    this->A.uinteger = 0;
    this->B.uinteger = 0;
    this->X.uinteger = 0;
    this->ctrl.all   = 0;
}

//============================================
// ArithmaticLogicUnit::process
// - Updates Decoder DataPorts based on CtrlPorts
//============================================
void ArithmaticLogicUnit::process(void){
    if(this->ctrl.alufp){
        switch(this->ctrl.aluop){
            case ALU_ADD :
                this->X.single = this->A.single + this->B.single;
            break;
            case ALU_SUB :
                this->X.single = this->A.single - this->B.single;
            break;
        }
    }else{
        switch(this->ctrl.aluop){
            case ALU_ADD :
                this->X.integer = this->A.integer + this->B.integer;
            break;
            case ALU_SUB :
                this->X.integer = this->A.integer - this->B.integer;
            break;
            case ALU_AND :
                this->X.integer = this->A.integer & this->B.integer;
            break;
            case ALU_OR  :
                this->X.integer = this->A.integer | this->B.integer;
            break;
            case ALU_XOR :
                this->X.integer = this->A.integer ^ this->B.integer;
            break;
            case ALU_SLL :
                this->X.integer = this->A.integer << this->B.integer;
            break;
            case ALU_SRL :
                this->X.integer = this->A.integer >> this->B.integer;
            break;
            case ALU_SRA :
            //C++ SRA Implementation inspired by:
            //https://stackoverflow.com/questions/53746160/how-to-implement-arithmetic-right-shift-in-c
            //Posted by user palotasb on 12/13/18
                this->X.integer = (this->A.integer < 0)?
                                  (~((~this->A.integer)>>this->B.integer)):
                                  (this->A.integer>>this->B.integer);
            break;
            case ALU_CMP :
                switch(this->ctrl.comp){ 
                    case 0b00: this->X.integer = (this->A.integer <  this->B.integer)?(1):(0); break;
                    case 0b01: this->X.integer = (this->A.integer == this->B.integer)?(1):(0); break;
                    case 0b10: this->X.integer = (this->A.integer != this->B.integer)?(1):(0); break;
                    case 0b11: this->X.integer = (this->A.integer >= this->B.integer)?(1):(0); break;
                }
            break;
            case ALU_CMPU:
                switch(this->ctrl.comp){ 
                    case 0b00: this->X.integer = ((this->A.uinteger) <  (this->B.uinteger))?(1):(0); break;
                    case 0b01: this->X.integer = ((this->A.uinteger) == (this->B.uinteger))?(1):(0); break;
                    case 0b10: this->X.integer = ((this->A.uinteger) != (this->B.uinteger))?(1):(0); break;
                    case 0b11: this->X.integer = ((this->A.uinteger) >= (this->B.uinteger))?(1):(0); break;
                }
            break;
        }
    }
}