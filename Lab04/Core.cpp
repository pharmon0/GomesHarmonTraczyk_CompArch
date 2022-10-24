/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Core Code */
#include "Core.h"

//============================================
// Constructors
//============================================
Core::Core(void){
  this->ctrl.all = 0;
  this->portI.memctrl.all = 0;
  this->portI.memctrl.memrsz = 0b11; //READ-ONLY (32b)
  this->portI.address = 0;
  this->portI.data = 0;
  this->portD.memctrl.all = 0;
  this->portD.address = 0x2FF; //top of stack
  this->portD.data = 0;
}

//============================================
// CPU Core Clock Cycle Process
//  This is the bulk of the system operation
//  returns false if halt instruction is executed
//============================================
bool Core::process(void){
  this->fdr.ctrl.all = 0;
  this->der.ctrl.all = 0;
  this->ewr.ctrl.all = 0;
 //---------------------------------------------
 //Fetch Stage (Interact with I Port and Update PC)
 //---------------------------------------------
    this->fdr.input.pc = this->pc;

   //Make Memory Request
    if(this->portI.memctrl.request && !this->portI.memctrl.memack){
      //waiting on memory, stall.
      this->ctrl.stallF = 1;
    }else if(this->ctrl.branched){
      if(!this->portI.memctrl.request){
        //CPU Branched, no active instruction fetch.
        //Get new data and wait.
        this->portI.memctrl.request = 1;
        this->portI.address = this->pc;
        this->ctrl.stallF = 1;
      }else if(this->portI.memctrl.memack){
        //CPU Branched, trash fetched data and request new.
        this->portI.memctrl.request = 1;
        this->portI.memctrl.memack  = 0;
        this->portI.address = this->pc;
        this->ctrl.branched = 0;
        this->ctrl.stallF = 1;
      }
    }else if(!this->portI.memctrl.request){
      //request data and wait
      this->portI.memctrl.request = 1;
      this->portI.address = this->pc;
      this->ctrl.stallF = 1;
    }else if(this->portI.memctrl.memack){
      //memory done. Load instruction to FDReg
      this->fdr.input.i = this->portI.data;
      this->portI.memctrl.request = 0;
      this->portI.memctrl.memack  = 0;
    } 
    
 //---------------------------------------------
 //Decode Stage (Decode Fetched instructions)
 //---------------------------------------------
    this->der.input.pc = this->fdr.output.pc;

   //run decoder
    this->dec.instruction = this->fdr.output.i;
    this->dec.process();
    this->der.input.rfctrl.all  = this->dec.rfctrl.all;
    this->der.input.aluctrl.all = this->dec.aluctrl.all;
    this->der.input.memctrl.all = this->dec.memctrl.all;
    this->der.input.bctrl.all   = this->dec.bctrl.all;

   //Process Immediate
    this->der.input.imm = immSignExtendShift(this->dec.immctrl.value,
                                             this->dec.immctrl.immsize,
                                             this->dec.immctrl.immshft);
   //Check Datahazards
    if(this->dec.rfctrl.selrs1 != X0){
      if(this->dec.rfctrl.selrs1 == this->der.output.rfctrl.selrd){
        if(this->dec.rfctrl.r1flop == this->der.output.rfctrl.rdflop)
          this->ctrl.stallD = 1; //instruction is using DEReg rd, stall.
      }else if(this->dec.rfctrl.selrs1 == this->ewr.output.rfctrl.selrd){
        if(this->dec.rfctrl.r1flop == this->ewr.output.rfctrl.rdflop)
          this->ctrl.stallD = 1; //instruction is using EWReg rd, stall.
      }
    }
    if(this->dec.rfctrl.selrs2 != X0){
      if(this->dec.rfctrl.selrs2 == this->der.output.rfctrl.selrd){
        if(this->dec.rfctrl.r2flop == this->der.output.rfctrl.rdflop)
          this->ctrl.stallD = 1; //instruction is using DEReg rd, stall.
      }else if(this->dec.rfctrl.selrs2 == this->ewr.output.rfctrl.selrd){
        if(this->dec.rfctrl.r2flop == this->ewr.output.rfctrl.rdflop)
          this->ctrl.stallD = 1; //instruction is using EWReg rd, stall.
      }
    }

 //---------------------------------------------
 //Execute Stage (Process Register File and ALU)
 //---------------------------------------------
   //Access Register File
    this->rf.rfctrl.selrs1 = this->der.output.rfctrl.selrs1;
    this->rf.rfctrl.selrs2 = this->der.output.rfctrl.selrs2;
    this->rf.rfctrl.r1flop = this->der.output.rfctrl.r1flop;
    this->rf.rfctrl.r2flop = this->der.output.rfctrl.r2flop;
    this->rf.processRead();
    this->ewr.input.rfctrl.all = this->der.output.rfctrl.all;
    this->ewr.input.rs2  = this->rf.rs2;

   //Run ALU
    if(this->der.output.bctrl.halt) return false; //HALT ON EXECUTE OF HALT INSTRUCTION
    if(this->der.output.aluctrl.aluasel){
      //use PC
      this->alu.A.integer = this->der.output.pc;
    }else{
      //use RS1
      this->alu.A = this->rf.rs1;
    }
    if(this->der.output.aluctrl.alubsel){
      //use Immediate
      this->alu.B.integer = this->der.output.imm;
    }else{
      //use RS2
      this->alu.B = this->rf.rs2;
    }
    this->alu.aluctrl.aluop   = this->der.output.aluctrl.aluop;
    this->alu.aluctrl.alucomp = this->der.output.aluctrl.alucomp;
    this->alu.aluctrl.alufp   = this->der.output.aluctrl.alufp;
    this->alu.process();
    this->ewr.input.aluX = this->alu.X;
    this->ctrl.stallE = alu.aluctrl.busy;

   //Run PCplus/Branch Logic
    bool branched = (this->der.output.bctrl.btype && (0b1 & this->alu.X.integer));
    this->ewr.input.pcplus = (branched)?
                             (this->der.output.pc + this->der.output.imm):
                             (this->der.output.pc + 4U);
    this->ewr.input.bctrl.branch = branched;

 //---------------------------------------------
 //Writeback Stage (Memory Access and Register File Writeback)
 //---------------------------------------------
    this->ctrl.branched = this->ewr.output.bctrl.branch || this->ewr.output.bctrl.jtype;
   //Port-D Memory Access
    this->portD.memctrl.memrsz = this->ewr.output.memctrl.memrsz;
    this->portD.memctrl.memwsz = this->ewr.output.memctrl.memwsz;
    if(this->ewr.output.memctrl.memwsz){
      //Store Instruction Detection
      this->portD.data = this->ewr.output.rs2.uinteger;
    }
    bool loading = false;
    if(this->ewr.output.memctrl.memrsz || this->ewr.output.memctrl.memwsz){
      //^^^ Check if memory access is occuring
      if(this->portD.memctrl.memack){
        //memory is done with operation
        //stop stalling and reset request.
        this->portD.memctrl.request = 0;
        this->portD.memctrl.memack = 0;
        if(this->ewr.output.memctrl.memrsz){
          //Load Instruction Detection
          loading = true;
        }
      }else if(this->portD.memctrl.request){
        //waiting on memory, stall
        this->ctrl.stallW = 1;
      }else{
        //make a request and stall
        this->portD.address = this->ewr.output.aluX.uinteger;
        this->portD.memctrl.request = 1;
        this->ctrl.stallW = 1;
      }
    }else{
      //no memory operation -> no stall
      this->ctrl.stallW = 0;
    }

   //writeback
    this->rf.rfctrl.rfwen  = this->ewr.output.rfctrl.rfwen
                          && !this->ctrl.stallW; //don't try to write while stalling for Mem
    this->rf.rfctrl.rdflop = this->ewr.output.rfctrl.rdflop;
    this->rf.rfctrl.selrd  = this->ewr.output.rfctrl.selrd;
    if(this->ewr.output.bctrl.jtype){
      this->rf.rd.integer = this->ewr.output.pcplus;
    }else if(loading){
      this->rf.rd.integer = this->portD.data;
    }else{
      this->rf.rd = this->ewr.output.aluX;
    }
    this->rf.processWrite();

    uint32_t pcbranch = 0;
    if(this->ewr.output.bctrl.branch){
      pcbranch = this->ewr.output.pcplus;
    }else if(this->ewr.output.bctrl.jtype){
      pcbranch = this->ewr.output.aluX.uinteger;
    }

 //Update PC and Advance Pipeline (or stall)
   //Pipeline Register Control
    this->fdr.ctrl.no_op = this->ctrl.stallF;
    this->der.ctrl.no_op = this->ctrl.stallD;
    this->ewr.ctrl.no_op = this->ctrl.stallE;
    this->fdr.ctrl.stall = this->ctrl.stallD
                        || this->ctrl.stallE
                        || this->ctrl.stallW;
    this->der.ctrl.stall = this->ctrl.stallE
                        || this->ctrl.stallW;
    this->ewr.ctrl.stall = this->ctrl.stallW;
    
   //Update PC
    if(((!this->ctrl.stallW) && (!this->ctrl.stallE)) 
     && ((!this->ctrl.stallD) && (!this->ctrl.stallF))){
      //Not stalling, okay to update PC
      if(this->ctrl.branched){
        //branched -> load branch
        this->pc = pcbranch;
        //flush pipeline
        this->fdr.ctrl.no_op = 1;
        this->der.ctrl.no_op = 1;
        this->ewr.ctrl.no_op = 1;
      }else{
        //Normal Operation, PC+=4
        this->pc += 4;
      }
    }

   //Update the pipeline Registers
    this->fdr.process();
    this->der.process();
    this->ewr.process();

    return true; //No Halt, Return True.
}