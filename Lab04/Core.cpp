/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Core Code */
#include "Core.h"

//============================================
// Constructors
//============================================
Core::Core(void){
  this->clock = 0;
  this->instcnt = 0;
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
  cout << "\n\n"
       << setfill('*') << setw(100) << "\n"
       << "Clock Cycle " << this->clock << "\n"
       << setfill('*') << setw(100) << " " << endl;
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
         << "  FETCH STAGE (CC=" << this->clock << ")\n"
         << setfill('=') << setw(80) << " " << endl;
    this->fdr.input.pc = this->pc;

   //Make Memory Request
    if(this->portI.memctrl.request && !this->portI.memctrl.memack){
      //waiting on memory, stall.
      this->ctrl.stallF = 1;
      cout << " Waiting for Port-I" << endl;
    }else if(this->ctrl.branched){
      if(!this->portI.memctrl.request){
        //CPU Branched, no active instruction fetch.
        //Get new data and wait.
        this->portI.memctrl.request = 1;
        this->portI.address = this->pc;
        this->ctrl.stallF = 1;
        cout << " CPU Branched, Port-I Starting new Fetch." << endl;
      }else if(this->portI.memctrl.memack){
        //CPU Branched, trash fetched data and request new.
        this->portI.memctrl.request = 1;
        this->portI.memctrl.memack  = 0;
        this->portI.address = this->pc;
        this->ctrl.branched = 0;
        this->ctrl.stallF = 1;
        cout << " CPU Branched, Port-I Trashing old fetch and Starting new Fetch." << endl;
      }
    }else if(!this->portI.memctrl.request){
      //request data and wait
      this->portI.memctrl.request = 1;
      this->portI.address = this->pc;
      this->ctrl.stallF = 1;
      cout << " Port-I Starting new Fetch." << endl;
    }else if(this->portI.memctrl.memack){
      //memory done. Load instruction to FDReg
      this->fdr.input.i = this->portI.data;
      this->portI.memctrl.request = 0;
      this->portI.memctrl.memack  = 0;
      this->ctrl.stallF = 0;
      cout << " Port-I Done Waiting" << endl;
    }

    cout << " Current PC : " << hexString(this->pc)
         << " (0b" << bitset<32>(this->pc) << ")" << endl;
    if(this->ctrl.stallF){
      cout << " Fetch Stage is Stalled!" << endl;
    }
    cout << " Instruction Fetched : " << hexString(this->fdr.input.i)
         << " (0b" << bitset<32>(this->fdr.input.i) << ")" << endl;
    
 //---------------------------------------------
 //Decode Stage (Decode Fetched instructions)
 //---------------------------------------------
    cout << setfill('=') << setw(80) << "\n"
         << "  DECODE STAGE (CC=" << this->clock << ")\n"
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
          cout << " DATAHAZARD! Execute Stage Rd matches Decode Stage RS1. Stalling Decode!" << endl;
        }
      }else if(this->dec.rfctrl.selrs1 == this->ewr.output.rfctrl.selrd){
        if(this->dec.rfctrl.r1flop == this->ewr.output.rfctrl.rdflop){
          this->ctrl.stallD = 1; //instruction is using EWReg rd, stall.
          cout << " DATAHAZARD! Writeback Stage Rd matches Decode Stage RS1. Stalling Decode!" << endl;
        }
      }
    }
    if(this->dec.rfctrl.selrs2 != X0){
      if(this->dec.rfctrl.selrs2 == this->der.output.rfctrl.selrd){
        if(this->dec.rfctrl.r2flop == this->der.output.rfctrl.rdflop){
          this->ctrl.stallD = 1; //instruction is using DEReg rd, stall.
          cout << " DATAHAZARD! Execute Stage Rd matches Decode Stage RS2. Stalling Decode!" << endl;
        }
      }else if(this->dec.rfctrl.selrs2 == this->ewr.output.rfctrl.selrd){
        if(this->dec.rfctrl.r2flop == this->ewr.output.rfctrl.rdflop){
          this->ctrl.stallD = 1; //instruction is using EWReg rd, stall.
          cout << " DATAHAZARD! Writeback Stage Rd matches Decode Stage RS2. Stalling Decode!" << endl;
        }
      }
    }

 //---------------------------------------------
 //Execute Stage (Process Register File and ALU)
 //---------------------------------------------
    cout << setfill('=') << setw(80) << "\n"
         << "  EXECUTE STAGE (CC=" << this->clock << ")\n"
         << setfill('=') << setw(80) << " " << endl;
   //Access Register File
    this->rf.rfctrl.selrs1 = this->der.output.rfctrl.selrs1;
    this->rf.rfctrl.selrs2 = this->der.output.rfctrl.selrs2;
    this->rf.rfctrl.r1flop = this->der.output.rfctrl.r1flop;
    this->rf.rfctrl.r2flop = this->der.output.rfctrl.r2flop;
    this->rf.processRead();
    this->ewr.input.rfctrl.all = this->der.output.rfctrl.all;
    this->ewr.input.rs2  = this->rf.rs2;
    cout << " RS1 : ";
    if(der.output.rfctrl.r1flop){cout << "F";}else{cout << "X";}
    cout << this->rf.rfctrl.selrs1 << " --> ";
    if(der.output.rfctrl.r1flop){cout << this->rf.rs1.single;}else{cout << this->rf.rs1.integer;}
    cout << " (" << hexString(this->rf.rs1.integer) << ")" << endl;
    cout << " RS2 : ";
    if(der.output.rfctrl.r2flop){cout << "F";}else{cout << "X";}
    cout << this->rf.rfctrl.selrs2 << " --> ";
    if(der.output.rfctrl.r2flop){cout << this->rf.rs2.single;}else{cout << this->rf.rs2.integer;}
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
    if(this->alu.aluctrl.busy) cout << " ALU PERFORMING FLOP! STALLING EXECUTE STAGE!" << endl;
    cout << " ALU Result : ";
    if(der.output.aluctrl.alufp){cout << this->alu.X.single;}else{cout << hexString(this->alu.X.integer);}
    cout << endl;

   //Run PCplus/Branch Logic
    bool branched = (this->der.output.bctrl.btype && (0b1 & this->alu.X.integer));
    this->ewr.input.pcplus = (branched)?
                             (this->der.output.pc + this->der.output.imm):
                             (this->der.output.pc + 4U);
    this->ewr.input.bctrl.branch = branched;
    if(branched){
      cout << " Branch Confirmed. Preparing to Branch." << endl;
      cout << " Branch Destination : " << hexString(this->ewr.input.pcplus) << endl;
    }

 //---------------------------------------------
 //Writeback Stage (Memory Access and Register File Writeback)
 //---------------------------------------------
    cout << setfill('=') << setw(80) << "\n"
         << "  WRITEBACK STAGE (CC=" << this->clock << ")\n"
         << setfill('=') << setw(80) << " " << endl;
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
      cout << "HALT INSTRUCTION IN WRITEBACK!!!\n --> EXITING CPU LOOP\n" << endl;
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
        //remove fetch, decode, and execute instructions from counter
        this->instcnt -= 3;
      }else{
        //Normal Operation, PC+=4
        this->pc += 4;
        this->instcnt++;
      }
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

    //DEBUG stop the clock after too long
    if(this->clock > 20)
      return false;

    return true; //No Halt, Return True.
}