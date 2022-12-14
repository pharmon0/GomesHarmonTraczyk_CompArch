/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Core Code */
#include "Core.h"

//============================================
// Constructors
//============================================
Core::Core(string name, uint32_t initialPC, uint32_t initialSP, uint8_t aluIntTickCount, uint8_t aluFlopTickCount){
  this->coreID = name;
  this->rf = RegisterFile(initialSP);
  this->alu = ALU(aluIntTickCount, aluFlopTickCount);
  this->pc = initialPC;
  this->ctrl.all = 0;
  this->portI.memctrl.all = 0;
  this->portI.memctrl.memrsz = 0b11; //READ-ONLY (32b)
  this->portI.address = 0;
  this->portI.data = 0;
  this->portD.memctrl.all = 0;
  this->portD.address = 0x2FF; //top of stack
  this->portD.data = 0;
  this->instructionCount = 0;
}

//============================================
// CPU Core Clock Cycle Process
//  This is the bulk of the system operation
//  returns false if halt instruction is executed
//============================================
bool Core::process(uint64_t tick){
  uint32_t clockCycle = tick / TICKS_PER_CLOCK;
  if(tick % TICKS_PER_CLOCK){
    //not a clock cycle tick
    return true; //no halt
  }
  cout << "\n" 
       << setfill('*') << setw(80) << "\n"
       << "RUNNING " << this->coreID << "'s CLOCK CYCLE #" << clockCycle << "\n"
       << setfill('*') << setw(80) << " " << endl;

  this->fdr.ctrl.all = 0;
  this->der.ctrl.all = 0;
  this->ewr.ctrl.all = 0;
  this->ctrl.stallW = 0;
  this->ctrl.stallE = 0;
  this->ctrl.stallD = 0;
  this->ctrl.stallF = 0;
 //---------------------------------------------
 //Fetch Stage (Interact with I Port and Update PC)
 //---------------------------------------------
    cout << setfill('=') << setw(80) << "\n"
         << "  FETCH STAGE\n"
         << setfill('=') << setw(80) << " " << endl;
    this->fdr.input.pc = this->pc;

   //Make Memory Request
    if(this->portI.memctrl.request && !this->portI.memctrl.memack){
      //waiting on memory, stall.
      this->ctrl.stallF = 1;
      cout << "Waiting for Port-I" << endl;
    }else if(this->ctrl.branched){
      if(!this->portI.memctrl.request){
        //CPU Branched, no active instruction fetch.
        //Get new data and wait.
        this->portI.memctrl.request = 1;
        this->portI.address = this->pc;
        this->ctrl.stallF = 1;
        cout << "CPU Branched, Port-I Starting new Fetch." << endl;
      }else if(this->portI.memctrl.memack){
        //CPU Branched, trash fetched data and request new.
        this->portI.memctrl.request = 1;
        this->portI.memctrl.memack  = 0;
        this->portI.address = this->pc;
        this->ctrl.branched = 0;
        this->ctrl.stallF = 1;
        cout << "CPU Branched, Port-I Trashing old fetch and Starting new Fetch." << endl;
      }
    }else if(!this->portI.memctrl.request){
      //request data and wait
      this->portI.memctrl.request = 1;
      this->portI.address = this->pc;
      this->ctrl.stallF = 1;
      cout << "Port-I Starting new Fetch." << endl;
    }else if(this->portI.memctrl.memack){
      //memory done. Load instruction to FDReg
      this->fdr.input.i = this->portI.data;
      this->portI.memctrl.request = 0;
      this->portI.memctrl.memack  = 0;
      this->ctrl.stallF = 0;
      cout << "Port-I Done Waiting" << endl;
    }

    cout << " Current PC : " << hexString(this->pc)
         << " (0b" << bitset<32>(this->pc) << ")" << endl;
    if(this->ctrl.stallF){
      cout << " Fetch Stage is Stalled!" << endl;
    }else{
      cout << " Instruction Fetched : " << hexString(this->fdr.input.i)
           << " (0b" << bitset<32>(this->fdr.input.i) << ")" << endl;
    }
    
 //---------------------------------------------
 //Decode Stage (Decode Fetched instructions)
 //---------------------------------------------
    cout << setfill('=') << setw(80) << "\n"
         << "  DECODE STAGE\n"
         << setfill('=') << setw(80) << " " << endl;
    this->der.input.pc = this->fdr.output.pc;

   //run decoder
    this->dec.instruction = this->fdr.output.i;
    this->dec.process(); //decoder prints its own stuff
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
        if(this->dec.rfctrl.r1flop == this->der.output.rfctrl.rdflop){
          this->ctrl.stallD = 1; //instruction is using DEReg rd, stall.
          cout << "DATAHAZARD! Execute Stage Rd matches Decode Stage RS1. Stalling Decode!" << endl;
        }
      }else if(this->dec.rfctrl.selrs1 == this->ewr.output.rfctrl.selrd){
        if(this->dec.rfctrl.r1flop == this->ewr.output.rfctrl.rdflop){
          this->ctrl.stallD = 1; //instruction is using EWReg rd, stall.
          cout << "DATAHAZARD! Writeback Stage Rd matches Decode Stage RS1. Stalling Decode!" << endl;
        }
      }
    }
    if(this->dec.rfctrl.selrs2 != X0){
      if(this->dec.rfctrl.selrs2 == this->der.output.rfctrl.selrd){
        if(this->dec.rfctrl.r2flop == this->der.output.rfctrl.rdflop){
          this->ctrl.stallD = 1; //instruction is using DEReg rd, stall.
          cout << "DATAHAZARD! Execute Stage Rd matches Decode Stage RS2. Stalling Decode!" << endl;
        }
      }else if(this->dec.rfctrl.selrs2 == this->ewr.output.rfctrl.selrd){
        if(this->dec.rfctrl.r2flop == this->ewr.output.rfctrl.rdflop){
          this->ctrl.stallD = 1; //instruction is using EWReg rd, stall.
          cout << "DATAHAZARD! Writeback Stage Rd matches Decode Stage RS2. Stalling Decode!" << endl;
        }
      }
    }

    //detect statistical noop
    this->der.input.noop = (this->fdr.output.i == 0);

 //---------------------------------------------
 //Execute Stage (Process Register File and ALU)
 //---------------------------------------------
    cout << setfill('=') << setw(80) << "\n"
         << "  EXECUTE STAGE\n"
         << setfill('=') << setw(80) << " " << endl;

   //Note NOOPS
    this->ewr.input.noop = this->der.output.noop;
    if(this->der.output.noop){
      cout << " NOOP DETECTED" << endl;
    }

   //Access Register File
    this->rf.rfctrl.selrs1 = this->der.output.rfctrl.selrs1;
    this->rf.rfctrl.selrs2 = this->der.output.rfctrl.selrs2;
    this->rf.rfctrl.r1flop = this->der.output.rfctrl.r1flop;
    this->rf.rfctrl.r2flop = this->der.output.rfctrl.r2flop;
    this->rf.processRead();
    this->ewr.input.rfctrl.all = this->der.output.rfctrl.all;
    this->ewr.input.rs2  = this->rf.rs2;

    cout << " RS1 : ";
    if(der.output.rfctrl.r1flop){
      cout << "F";
    }else{
      cout << "X";
    }
    cout << this->rf.rfctrl.selrs1 << " --> ";
    if(der.output.rfctrl.r1flop){
      cout << this->rf.rs1.single;
    }else{
      cout << this->rf.rs1.integer;
    }
    cout << " (" << hexString(this->rf.rs1.integer) << ")" << endl;
    cout << " RS2 : ";
    if(der.output.rfctrl.r2flop){
      cout << "F";
    }else{
      cout << "X";
    }
    cout << this->rf.rfctrl.selrs2 << " --> ";
    if(der.output.rfctrl.r2flop){
      cout << this->rf.rs2.single;
    }else{
      cout << this->rf.rs2.integer;
    }
    cout << " (" << hexString(this->rf.rs2.integer) << ")" << endl;

   //Run ALU
    if(this->der.output.aluctrl.aluasel){
      //use PC
      this->alu.A.integer = this->der.output.pc;
      cout << " ALU A gets PC (" << hexString(this->der.output.pc) << endl;
    }else{
      //use RS1
      this->alu.A = this->rf.rs1;
      cout << " ALU A gets RS1" << endl;
    }
    
    if(this->der.output.aluctrl.alubsel){
      //use Immediate
      this->alu.B.integer = this->der.output.imm;
      cout << " ALU B gets Immediate" << hexString(this->der.output.imm) << endl;
    }else{
      //use RS2
      this->alu.B = this->rf.rs2;
      cout << " ALU B gets RS2" << endl;
    }
    
    this->alu.aluctrl.aluop   = this->der.output.aluctrl.aluop;
    this->alu.aluctrl.alucomp = this->der.output.aluctrl.alucomp;
    this->alu.aluctrl.alufp   = this->der.output.aluctrl.alufp;

    this->alu.process();
    
    this->ewr.input.aluX = this->alu.X;
    this->ctrl.stallE = alu.aluctrl.busy;
    
    if(this->alu.aluctrl.busy) cout << " ALU BUSY! STALLING EXECUTE STAGE!" << endl;
    
    cout << " ALU Result : ";
    cout << hexString(this->alu.X.integer) << " (";
    if(der.output.aluctrl.alufp){
      cout << this->alu.X.single;
    }else{
      cout << this->alu.X.integer;
    }
    cout << ")" << endl;

   //Run PCplus/Branch Logic
    bool branching = (this->der.output.bctrl.btype && (0b1 & this->alu.X.integer));
    this->ewr.input.pcplus = (branching)?
                             (this->der.output.pc + this->der.output.imm):
                             (this->der.output.pc + 4U);
    this->ewr.input.bctrl.branch = branching;
    this->ewr.input.bctrl.jtype = this->der.output.bctrl.jtype;
    this->ewr.input.bctrl.halt = this->der.output.bctrl.halt;
    
    if(this->der.output.bctrl.btype){
      cout << " B-type Branch Attempted..." << endl;
      if(branching){
        cout << "  Branch Success! destination = " << hexString(this->ewr.input.pcplus) << endl;
        cout << "  Branch will be taken on writeback stage." << endl;
      }else{
        cout << "  Branch not taken!" << endl;
      }
    }
    if(this->der.output.bctrl.jtype){
      cout << " Processing J-type branch..." << endl;
      cout << "  destination = " << hexString(this->ewr.input.aluX.uinteger) << endl;
    }

    //forward memory controls
    this->ewr.input.memctrl.all = this->der.output.memctrl.all;

 //---------------------------------------------
 //Writeback Stage (Memory Access and Register File Writeback)
 //---------------------------------------------
    cout << setfill('=') << setw(80) << "\n"
         << "  WRITEBACK STAGE\n"
         << setfill('=') << setw(80) << " " << endl;

   //Port-D Memory Access
    cout << " Memctrl :: memrsz : " << (int)this->ewr.output.memctrl.memrsz
         << " memwsz : " << (int)this->ewr.output.memctrl.memwsz
         << " memrsgn : " << (int)this->ewr.output.memctrl.memrsgn << endl;
    this->portD.memctrl.memrsz = this->ewr.output.memctrl.memrsz;
    this->portD.memctrl.memwsz = this->ewr.output.memctrl.memwsz;
    if(this->ewr.output.memctrl.memwsz){
      //Store Instruction Detection
      this->portD.data = this->ewr.output.rs2.uinteger;
      cout << " Attempting a STORE | address : " << hexString(this->ewr.output.aluX.uinteger)
           << " | data : " << hexString(this->ewr.output.rs2.uinteger) << endl;
    }
    if(this->ewr.output.memctrl.memrsz){
      //Load Instruction Detection
      cout << " Attempting a LOAD | address : " << hexString(this->ewr.output.aluX.uinteger)
           << " | Rd : " << this->ewr.output.rfctrl.selrd << endl; 
    }
    bool loading = false;
    if(this->ewr.output.memctrl.memrsz || this->ewr.output.memctrl.memwsz){
      //^^^ Check if memory access is occuring
      if(this->portD.memctrl.memack){
        //memory is done with operation
        //stop stalling and reset request.
        this->portD.memctrl.request = 0;
        this->portD.memctrl.memack = 0;
        cout << " Port-D Memory Done.";
        if(this->ewr.output.memctrl.memrsz){
          //Load Instruction Detection
          loading = true;
          cout << " Data Loaded!";
        }
        cout << endl;
      }else if(this->portD.memctrl.request){
        //waiting on memory, stall
        this->ctrl.stallW = 1;
        cout << " Port-D Waiting!" << endl;
      }else{
        //make a request and stall
        this->portD.address = this->ewr.output.aluX.uinteger;
        this->portD.memctrl.request = 1;
        this->ctrl.stallW = 1;
        cout << " Port-D Sending an Access Request!" << endl;
      }
    }else{
      //no memory operation -> no stall
      this->ctrl.stallW = 0;
      cout << " Port-D No Memory Access" << endl;
    }

    if(this->ctrl.stallW) cout << " WAITING ON MEMORY PORT-D! STALLING WRITEBACK!" << endl;

   //check for halt
    if(this->ewr.output.bctrl.halt){
      //HALT ON EXECUTE OF HALT INSTRUCTION
      cout << "\nHALT INSTRUCTION IN WRITEBACK!!!\n --> EXITING CPU LOOP\n" << endl;
      cout << setfill('*') << setw(80) << "\n"
        << "END OF " << this->coreID << "'s CLOCK CYCLE #" << clockCycle << "\n"
        << setfill('*') << setw(80) << " \n" << endl;
      return false;
    }

   //writeback
    this->rf.rfctrl.rfwen  = this->ewr.output.rfctrl.rfwen
                          && !this->ctrl.stallW; //don't try to write while stalling for Mem
    cout << " Register File Write Enable: " << rf.rfctrl.rfwen << endl;
    this->rf.rfctrl.rdflop = this->ewr.output.rfctrl.rdflop;
    this->rf.rfctrl.selrd  = this->ewr.output.rfctrl.selrd;
    if(this->ewr.output.bctrl.jtype){
      this->rf.rd.integer = this->ewr.output.pcplus;
      cout << " rd <- pc+4" << endl;
    }else if(loading){
      this->rf.rd.integer = this->portD.data;
      cout << " rd <- load from memory" << endl;
    }else{
      this->rf.rd = this->ewr.output.aluX;
      cout << " rd <- ALU result" << endl;
    }
    this->rf.processWrite();

    uint32_t pcbranch = 0;
    cout << "DEBUG cbranch : " << pcbranch << endl;
    if(this->ewr.output.bctrl.branch){
      cout << "DEBUG - 1 - pcbranch : " << pcbranch << endl;
      pcbranch = this->ewr.output.pcplus;
    }else if(this->ewr.output.bctrl.jtype){
      cout << "DEBUG - 2 - pcbranch : " << pcbranch << endl;
      pcbranch = this->ewr.output.aluX.uinteger;
    }
    this->ctrl.branched = this->ewr.output.bctrl.branch || this->ewr.output.bctrl.jtype;
    if(this->ewr.output.bctrl.jtype || this->ewr.output.bctrl.branch){
      this->ctrl.stallF = 0;
      this->ctrl.stallD = 0;
      this->ctrl.stallE = 0;
      this->ctrl.stallW = 0;
    }
    cout << "pcbranch : " << pcbranch << endl;

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
      cout << "PC IS NOT STALLED!" << endl;
      //Not stalling, okay to update PC
      if(this->ctrl.branched){
        cout << "PC IS Branching!" << endl;
        //branched -> load branch
        cout << "\tPC = " << this->pc << endl;
        this->pc = pcbranch;
        cout << "\tPC = " << this->pc << endl;
        //flush pipeline
        this->fdr.ctrl.no_op = 1;
        this->der.ctrl.no_op = 1;
        this->ewr.ctrl.no_op = 1;
      }else{
        cout << "PC IS Normal!" << endl;
        //Normal Operation, PC+=4
        cout << "\tPC = " << this->pc << endl;
        this->pc += 4;
        cout << "\tPC = " << this->pc << endl;
      }
    } else{
      cout << "PC IS STALLED!" << endl;
    }

   //Update the pipeline Registers
    this->fdr.process();
    this->der.process();
    this->ewr.process();

    cout << "CPU Status:"
         << " stallF : " << bitset<1>(this->ctrl.stallF)
         << " stallD : " << bitset<1>(this->ctrl.stallD)
         << " stallE : " << bitset<1>(this->ctrl.stallE)
         << " stallW : " << bitset<1>(this->ctrl.stallW)
         << endl;

    //DEBUG PC LIMIT
    //if(this->pc > 0x1FF){
    //if(this->pc > 0x2C){
    //  cout << "\nDEBUG PC OVERFLOW!!!\n" << endl;
    //  return false;
    //}

    //Instruction Counter for statistics
    if(!this->ctrl.stallW){
      //not stalled
      if(!this->ewr.output.noop){
        //not a noop from execute
        this->instructionCount++;
      }
    }

    cout << setfill('*') << setw(80) << "\n"
       << "END OF " << this->coreID << "'s CLOCK CYCLE #" << clockCycle << "\n"
       << setfill('*') << setw(80) << " \n" << endl;
    return true; //No Halt, Return True.
}