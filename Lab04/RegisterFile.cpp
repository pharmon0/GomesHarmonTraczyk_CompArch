/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Register File Function Code */
#include "RegisterFile.h"

//============================================
// RegisterFile Constructor
//============================================
RegisterFile::RegisterFile(void){
    for(uint16_t i = 0; i < 32; i++){
        this->iBank[i] = 0U;
        this->fBank[i] = 0.0;
    }
    this->rfctrl.all = 0;
    this->rs1.uinteger = 0U;
    this->rs2.uinteger = 0U;
    this->rd.uinteger = 0U;
    this->iBank[XSP] = 0x2FF;//SP Range 0x200 - 0x2FF
}

//============================================
// RegisterFile::processRead
// - processes data through the RF dataports
//============================================
void RegisterFile::processRead(void){
    //resolve whether rs1 is float or int
    if(this->rfctrl.r1flop){
        this->rs1.single = this->fBank[this->rfctrl.selrs1];
    }else{
        this->rs1.integer = this->iBank[this->rfctrl.selrs1];
    }
    //resolve whether rs2 is float or int
    if(this->rfctrl.r2flop){
        this->rs2.single = this->fBank[this->rfctrl.selrs2];
    }else{
        this->rs1.integer = this->iBank[this->rfctrl.selrs2];
    }
}

//============================================
// RegisterFile::processWrite
// - processes data through the RF dataports
//============================================
void RegisterFile::processWrite(void){
    //don't write if rfwen==0
    //don't write to rd = X0
    if((!this->rfctrl.rfwen) || (this->rfctrl.selrd == 0)) return;
    //resolve float vs int
    if(this->rfctrl.rdflop){
        this->fBank[this->rfctrl.selrd] = this->rd.single;
    }else{
        this->iBank[this->rfctrl.selrd] = this->rd.integer;
    }
}