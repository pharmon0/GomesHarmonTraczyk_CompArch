# Lab 4
## Class Briefs
### RegisterFile
Holds an array of 32 integers and an array of 32 floats. This is the Register file for the processor.
 - private
   - uint32_t iBank[32] : 32 member uint32_t array. This is the integer register file.
   - float fBank[32] : 32 member float array. This is the floating point register file.
 - dataPorts
   - data32_t rs1 : output data from source register 1. Datatype is ambiguous.
   - data32_t rs2 : output data from source register 2. Datatype is ambiguous.
   - data32_t rd : input data to destination register. Datatype is ambiguous.
 - ctrlPorts
   - (5bit) selrs1 : register address for rs1.
   - (5bit) selrs2 : register address for rs2.
   - (5bit) selrd : register address for rd.
   - (1bit) rfwen : register file write enable. disables rd writeback if 0.
   - (1bit) r1flop : sets rs1 as a float value.
   - (1bit) r2flop : sets rs2 as a float value.
   - (1bit) rdflop : sets rd as a float value.
 - functions
   - void processRead(void) : tick the register file read ports
###