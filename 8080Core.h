#ifndef CORE_EMU
#define CORE_EMU

#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

class State8080 {
private:
    unsigned char reg_A;
    unsigned char reg_B;
    unsigned char reg_C;
    unsigned char reg_D;
    unsigned char reg_E;
    unsigned char reg_H;
    unsigned char reg_L;
    unsigned char reg_PSW;
    unsigned int pc;
    unsigned int sp;
    bool flagSign;
    bool flagZero;
    bool flagAuxCarry;
    bool flagParity;
    bool flagCarry;
    bool interruptsEnabled;

    unsigned char* instruction; //Instruction with operands that will be processed
    unsigned int romDataSize; //Size of rom file to offset stack pointer with

    const std::vector<std::string> OPCODES = {
            "NOP",
            "LXI B,d16",
            "STAX B",
            "INX B",
            "INR B",
            "DCR B",
            "MVI B,d8",
            "RLC",
            "*NOP",
            "DAD B",
            "LDAX B",
            "DCX B",
            "INR C",
            "DCR C",
            "MVI C,d8",
            "RRC",
            "*NOP",
            "LXI D,d16",
            "STAX D",
            "INX D",
            "INR D",
            "DCR D",
            "MVI D,d8",
            "RAL",
            "*NOP",
            "DAD D",
            "LDAX D",
            "DCX D",
            "INR E",
            "DCR E",
            "MVI E,d8",
            "RAR",
            "*NOP",
            "LXI H,d16",
            "SHLD a16",
            "INX H",
            "INR H",
            "DCR H",
            "MVI H,d8",
            "DAA",
            "*NOP",
            "DAD H",
            "LHLD a16",
            "DCX H",
            "INR L",
            "DCR L",
            "MVI L,d8",
            "CMA",
            "*NOP",
            "LXI SP,d16",
            "STA a16",
            "INX SP",
            "INR M",
            "DCR M",
            "MVI M,d8",
            "STC",
            "*NOP",
            "DAD SP",
            "LDA a16",
            "DCX SP",
            "INR A",
            "DCR A",
            "MVI A,d8",
            "CMC",
            "MOV B,B",
            "MOV B,C",
            "MOV B,D",
            "MOV B,E",
            "MOV B,H",
            "MOV B,L",
            "MOV B,M",
            "MOV B,A",
            "MOV C,B",
            "MOV C,C",
            "MOV C,D",
            "MOV C,E",
            "MOV C,H",
            "MOV C,L",
            "MOV C,M",
            "MOV C,A",
            "MOV D,B",
            "MOV D,C",
            "MOV D,D,",
            "MOV D,E",
            "MOV D,H",
            "MOV D,L",
            "MOV D,M",
            "MOV D,A",
            "MOV E,B",
            "MOV E,C",
            "MOV E,D",
            "MOV E,E",
            "MOV E,H",
            "MOV E,L",
            "MOV E,M",
            "MOV E,A",
            "MOV H,B",
            "MOV H,C",
            "MOV H,D",
            "MOV H,E",
            "MOV H,H",
            "MOV H,L",
            "MOV H,M",
            "MOV H,A",
            "MOV L,B",
            "MOV L,C",
            "MOV L,D",
            "MOV L,E",
            "MOV L,H",
            "MOV L,L",
            "MOV L,M",
            "MOV L,A",
            "MOV M,B",
            "MOV M,C",
            "MOV M,D",
            "MOV M,E",
            "MOV M,H",
            "MOV M,L",
            "HLT",
            "MOV M,A",
            "MOV A,B",
            "MOV A,C",
            "MOV A,D",
            "MOV A,E",
            "MOV A,H",
            "MOV A,L",
            "MOV A,M",
            "MOV A,A",
            "ADD B",
            "ADD C",
            "ADD D",
            "ADD E",
            "ADD H",
            "ADD L",
            "ADD M",
            "ADD A",
            "ADC B",
            "ADC C",
            "ADC D",
            "ADC E",
            "ADC H",
            "ADC L",
            "ADC M",
            "ADC A",
            "SUB B",
            "SUB C",
            "SUB D",
            "SUB E",
            "SUB H",
            "SUB L",
            "SUB M",
            "SUB A",
            "SBB B",
            "SBB C",
            "SBB D",
            "SBB E",
            "SBB H",
            "SBB L",
            "SBB M",
            "SBB A",
            "ANA B",
            "ANA C",
            "ANA D",
            "ANA E",
            "ABA H",
            "ANA L",
            "ANA M",
            "ANA A",
            "XRA B",
            "XRA C",
            "XRA D",
            "XRA E",
            "XRA H",
            "XRA L",
            "XRA M",
            "XRA A",
            "ORA B",
            "ORA C",
            "ORA D",
            "ORA E",
            "ORA H",
            "ORA L",
            "ORA M",
            "ORA A",
            "CMP B",
            "CMP C",
            "CMP D",
            "CMP E",
            "CMP H",
            "CMP L",
            "CMP M",
            "CMP A",
            "RNZ",
            "POP B",
            "JNZ A16",
            "JMP A16",
            "CNZ A16",
            "PUSH B",
            "ADI D8",
            "RST 0",
            "RZ",
            "RET",
            "JZ A16",
            "*JMP A16",
            "CZ A16",
            "CALL A16",
            "ACI D8",
            "RST 1",
            "RNC",
            "POP D",
            "JNC A16",
            "OUT D8",
            "CNC A16",
            "PUSH D",
            "SUI D8",
            "RST 2",
            "RC",
            "*RET",
            "JC A16",
            "IN D8",
            "CC A16",
            "*CALL A16",
            "SBI D8",
            "RST 3",
            "RPO",
            "POP H",
            "JP0 A16",
            "XTHL",
            "CPO A16",
            "PUSH H",
            "ANI D8",
            "RST 4",
            "RPE",
            "PCHL",
            "JPE A16",
            "XCHG",
            "CPE A16",
            "*CALL A16",
            "XRI D8",
            "RST 5",
            "RP",
            "POP PSW",
            "JP A16",
            "DI",
            "CP A16",
            "PUSH PSW",
            "ORI D8",
            "RST 6",
            "RM",
            "SPHL",
            "JM A16",
            "EI",
            "CM A16",
            "*CALL A16",
            "CPI D8",
            "RST 7"

    };


    //instruction: 2 byte ADI instruction
    void ADI(const unsigned char *instruction);

    //instruction:: 1 byte ANA instruction
    //memory:: memory to read from
    void ANA(const unsigned char *instruction, unsigned char *memory);
    //instruction: 2 byte ANI instruction
    void ANI(const unsigned char *instruction);

    void CALL(const unsigned char *instruction, unsigned char* memory);

    //instruction: 2 byte CPI instruction
    void CPI(const unsigned char *instruction);

    //instruction: 1 byte DAD instruction
    void DAD(const unsigned char *instruction);
    //instruction: 1 byte DCR instruction
    void DCR(const unsigned char *instruction);

    //Enables interrupts
    void EI();
    //instruction: 1 byte INX instruction
    void INX(const unsigned char *instruction);

    //instruction: 3 byte JMP/JNZ instruction
    void JMP(const unsigned char *instruction);

    //instruction: 3 byte LDA instruction
    //memory: memory to read from
    void LDA(const unsigned char *instruction, unsigned char *memory);
    //instruction: 1 byte LDAX instruction
    //memory: memory to read/write data
    void LDAX(const unsigned char *instruction, unsigned char *memory);

    //instruction: 3 byte LXI instruction
    void LXI(const unsigned char *instruction);
    //instruction: 1 byte MOV instruction
    //memory: memory to read/write data
    void MOV(const unsigned char *instruction, unsigned char *memory);

    //instruction: 2 byte MVI instruction
    //memory: memory to read/write from
    void MVI(const unsigned char *instruction, unsigned char *memory);

    //instruction: 2 byte OUT instruction
    //TODO: What does this do?
    void OUT(const unsigned char *instruction);
    //instruction: 1 byte POP instruction
    //memory: memory to read/write from
    void POP(const unsigned char *instruction, unsigned char *memory);
    //instruction: 1 byte PUSH instruction
    //memory: memory to read/write from
    void PUSH(const unsigned char *instruction, unsigned char *memory);

    //memory: memory to read/write from
    void RET(unsigned char *memory);

    void RRC();

    //instruction: 3 byte STA instruction
    //memory: memory to write to
    void STA(const unsigned char *instruction, unsigned char *memory);

    void XCHG();

    //instruction: 1 byte XRA instruction
    //memory: memory to read from
    void XRA(const unsigned char *instruction, unsigned char *memory);
    //Error called when unimplemented instruction is run
    void UnimplementedInstruction();

public:
    //Resets all members of cpuState to 0
    void InitCPU();

    State8080();
    State8080(unsigned int romSize);
    //Returns the opcode for the buffer at the current program counter(pc)
    std::string GetOpcode(unsigned char opcode);

    //Basepoint for emulating a CPU instruction
    //Automatically runs the instruction in data specified by the program counter
    //and modifies memory as necessary
    void RunInstruction(unsigned char* memory);

    int breakpoint; //Halts processor after this number of instructions
                    //used for debugging

};
#endif