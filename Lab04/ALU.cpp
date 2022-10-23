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
    this->A=0;
    this->B=0;
    this->X=0;
    this->ctrl.all = 0;
}

//============================================
// ArithmaticLogicUnit::process
// - Updates Decoder DataPorts based on CtrlPorts
//============================================
void ArithmaticLogicUnit::process(void){
    switch(this->ctrl.){
      case :
        switch(this->ctrl.aluop)
      break;
      default:
        switch(this->ctrl.aluop){
            case ALU_ADD :
                this->X = this->A + this->B;
            break;
            case ALU_SUB :
                this->X = this->A - this->B;
            break;
            case ALU_AND :
                this->X = this->A & this->B;
            break;
            case ALU_OR  :
                this->X = this->A | this->B;
            break;
            case ALU_XOR :
                this->X = this->A ^ this->B;
            break;
            case ALU_SLL :
                this->X = this->A << this->B;
            break;
            case ALU_SRL :
                this->X = this->A >> this->B;
            break;
            case ALU_SRA :
            //C++ SRA Implementation inspired by:
            //https://stackoverflow.com/questions/53746160/how-to-implement-arithmetic-right-shift-in-c
            //Posted by user palotasb on 12/13/18
                this->X = (this->A < 0)?(~((~this->A)>>this->B)):(this->A>>this->B);
            break;
            case ALU_CMP :
                switch(this->ctrl.comp){ 
                    case 0b00: this->X = (this->A <  this->B)?(1):(0); break;
                    case 0b01: this->X = (this->A == this->B)?(1):(0); break;
                    case 0b10: this->X = (this->A != this->B)?(1):(0); break;
                    case 0b11: this->X = (this->A >= this->B)?(1):(0); break;
                }
            break;
            case ALU_CMPU:
                switch(this->ctrl.comp){ 
                    case 0b00: this->X = (((uint32_t)this->A) <  ((uint32_t)this->B))?(1):(0); break;
                    case 0b01: this->X = (((uint32_t)this->A) == ((uint32_t)this->B))?(1):(0); break;
                    case 0b10: this->X = (((uint32_t)this->A) != ((uint32_t)this->B))?(1):(0); break;
                    case 0b11: this->X = (((uint32_t)this->A) >= ((uint32_t)this->B))?(1):(0); break;
                }
            break;
        }
    }
}