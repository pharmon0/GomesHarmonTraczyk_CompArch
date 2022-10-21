/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Register File Function Code */
#include "RegisterFile.h"

//============================================
// RegisterFile Constructor
//============================================
RegisterFile::RegisterFile(void){
    for(uint16_t i = 0; i < 32; i++){
        this->iBank[i] = 0x00000000;
        this->fBank[i] = 0.0;
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
    //load rs1 and rs2 to new values
    if(this->ctrl.r1flop){
        this->rs1.single = this->fBank[this->ctrl.selrs1];
    }else{
        this->rs1.uinteger = this->iBank[this->ctrl.selrs1];
    }
    if(this->ctrl.r2flop){
        this->rs2.single = this->fBank[this->ctrl.selrs2];
    }else{
        this->rs1.uinteger = this->iBank[this->ctrl.selrs2];
    }
}

//============================================
// RegisterFile::processWrite
// - processes data through the RF dataports
//============================================
void RegisterFile::processWrite(void){
    //if write enabled, store new rd value
    if(!this->ctrl.rfwen) return;
    if(this->ctrl.rdflop){
        this->fBank[this->ctrl.selrd] = this->rd.single;
    }else{
        this->iBank[this->ctrl.selrd] = this->rd.uinteger;
    }
}