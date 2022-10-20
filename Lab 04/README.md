# Lab 4
## Module Briefs
These will be filled in as they are reworked from Lab03 to Lab04.
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
### Datatypes.h
Provides a collection of datatypes with various uses.
 - datatypes
   - instruction_t
      This is a union datatype that contains a uint32_t and a large set of bitmapped values. These bitmaps correspond to the fields in various RISCV instruction types. This allows for easy reading of instruction data in the decode stage.
   - data32_t
      This is a union datatype that contains a uint32_t, an int32_t, and a float. This allows for easy handling of type-ambiguous data busses.