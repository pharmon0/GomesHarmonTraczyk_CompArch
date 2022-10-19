/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Register File Function Code */
#include "RegisterFile.h"

//============================================
// RegisterFile Constructor
//============================================
RegisterFile::RegisterFile(void){
    for(uint16_t i = 0; i < 32; i++){
        this->bank[i] = 0x00000000;
    }
    this->ctrl.all = 0;
}

//============================================
// RegisterFile::processRead
// - processes data through the RF dataports
//============================================
void RegisterFile::processRead(void){
    //load rs1 and rs2 to new values
    this->rs1 = this->bank[this->ctrl.selrs1];
    this->rs2 = this->bank[this->ctrl.selrs2];
}

//============================================
// RegisterFile::processWrite
// - processes data through the RF dataports
//============================================
void RegisterFile::processWrite(int32_t rd){
    //if write enabled, store new rd value
    if(this->ctrl.rfwen) this->bank[this->ctrl.selrd] = rd;
}