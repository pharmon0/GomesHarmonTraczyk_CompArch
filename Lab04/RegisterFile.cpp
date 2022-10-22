/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Register File Function Code */
#include "RegisterFile.h"

//============================================
// RegisterFile Constructor
//============================================
RegisterFile::RegisterFile(void){
    for(uint16_t i = 0; i < 32; i++){
        this->iBank[i].integer = 0U;
        this->fBank[i].integer = 0U;
    }
    this->ctrl.all = 0;
    this->rs1.uinteger = 0U;
    this->rs2.uinteger = 0U;
    this->rd.uinteger = 0U;
}

//============================================
// RegisterFile::processRead
// - processes data through the RF dataports
//============================================
void RegisterFile::processRead(void){
    //resolve whether rs1 is float or int
    if(this->ctrl.r1flop){
        this->rs1 = this->fBank[this->ctrl.selrs1];
    }else{
        this->rs1 = this->iBank[this->ctrl.selrs1];
    }
    //resolve whether rs2 is float or int
    if(this->ctrl.r2flop){
        this->rs2 = this->fBank[this->ctrl.selrs2];
    }else{
        this->rs1 = this->iBank[this->ctrl.selrs2];
    }
}

//============================================
// RegisterFile::processWrite
// - processes data through the RF dataports
//============================================
void RegisterFile::processWrite(void){
    //don't write if rfwen==0
    //don't write to rd = X0
    if((!this->ctrl.rfwen) || (this->ctrl.selrd == 0)) return;
    //resolve float vs int
    if(this->ctrl.rdflop){
        this->fBank[this->ctrl.selrd] = this->rd;
    }else{
        this->iBank[this->ctrl.selrd] = this->rd;
    }
}