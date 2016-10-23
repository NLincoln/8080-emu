#include "8080Core.h"
#include <cmath>
std::string State8080::GetOpcode(unsigned char opcode)
{
    return OPCODES[opcode];
}

State8080::State8080()
{
    instruction = new unsigned char[3];
    romDataSize = 0;
}
State8080::State8080(unsigned int romSize)
{
    instruction = new unsigned char[3];
    romDataSize = romSize;
    breakpoint = -1;
}
void State8080::InitCPU()
{
    reg_A = 0;
    reg_B = 0;
    reg_C = 0;
    reg_D = 0;
    reg_E = 0;
    reg_H = 0;
    reg_L = 0;
    reg_PSW = 0;
    pc = 0;
    flagSign = false;
    flagZero = false;
    flagAuxCarry = false;
    flagCarry = false;
    flagParity = false;
    interruptsEnabled = false;
}
void State8080::RunInstruction(unsigned char* memory) {

    //Fill instruction buffer
    instruction[0] = memory[pc];
    instruction[1] = memory[pc + 1];
    instruction[2] = memory[pc + 2];

    std::cout << "PC is: " << pc << " ";
    //Increment pc before instruction in case of JMP
    pc++;

    std::cout<<GetOpcode(instruction[0])
             << std::endl;

    //Go to the proper instruction
    switch(instruction[0])
    {
        case(0x00):
            break;
        case(0x01):
        case(0x11):
        case(0x21):
        case(0x31):
            LXI(instruction);
            break;
        case(0x06):
        case(0x0e):
        case(0x16):
        case(0x1e):
        case(0x26):
        case(0x2e):
        case(0x36):
        case(0x3e):
            MVI(instruction, memory);
            break;
        case(0x0f):
            RRC();
            break;
        case(0x0a):
        case(0x1a):
            LDAX(instruction, memory);
        case(0x03):
        case(0x13):
        case(0x23):
        case(0x33):
            INX(instruction);
            break;
        case(0x05):
        case(0x0d):
        case(0x15):
        case(0x1d):
        case(0x25):
        case(0x2d):
        case(0x35):
        case(0x3d):
            DCR(instruction);
            break;
        case(0x09):
        case(0x19):
        case(0x29):
        case(0x39):
            DAD(instruction);
            break;
        case(0x32):
            STA(instruction, memory);
            break;
        case(0x3a):
            LDA(instruction, memory);
            break;
        case(0x40):
        case(0x41):
        case(0x42):
        case(0x43):
        case(0x44):
        case(0x45):
        case(0x46):
        case(0x47):
        case(0x48):
        case(0x49):
        case(0x4a):
        case(0x4b):
        case(0x4c):
        case(0x4d):
        case(0x4e):
        case(0x4f):
        case(0x50):
        case(0x51):
        case(0x52):
        case(0x53):
        case(0x54):
        case(0x55):
        case(0x56):
        case(0x57):
        case(0x58):
        case(0x59):
        case(0x5a):
        case(0x5b):
        case(0x5c):
        case(0x5d):
        case(0x5e):
        case(0x5f):
        case(0x60):
        case(0x61):
        case(0x62):
        case(0x63):
        case(0x64):
        case(0x65):
        case(0x66):
        case(0x67):
        case(0x68):
        case(0x69):
        case(0x6a):
        case(0x6b):
        case(0x6c):
        case(0x6d):
        case(0x6e):
        case(0x6f):
        case(0x70):
        case(0x71):
        case(0x72):
        case(0x73):
        case(0x74):
        case(0x75):
        case(0x77):
        case(0x78):
        case(0x79):
        case(0x7a):
        case(0x7b):
        case(0x7c):
        case(0x7d):
        case(0x7e):
        case(0x7f):
            MOV(instruction, memory);
            break;
        case(0xa0):
        case(0xa1):
        case(0xa2):
        case(0xa3):
        case(0xa4):
        case(0xa5):
        case(0xa6):
        case(0xa7):
            ANA(instruction, memory);
            break;
        case(0xa8):
        case(0xa9):
        case(0xaa):
        case(0xab):
        case(0xac):
        case(0xad):
        case(0xae):
        case(0xaf):
            XRA(instruction, memory);
            break;
        case(0xe6):
            ANI(instruction);
            break;
        case(0xc6):
            ADI(instruction);
            break;
        case(0xc1):
        case(0xd1):
        case(0xe1):
        case(0xf1):
            POP(instruction, memory);
            break;
        case(0xc2):
        case(0xc3):
            JMP(instruction);
            break;
        case(0xc9):
        case(0xd9):
            RET(memory);
            break;
        case(0xd3):
            OUT(instruction);
            break;
        case(0xc5):
        case(0xd5):
        case(0xe5):
        case(0xf5):
            PUSH(instruction, memory);
            break;
        case(0xeb):
            XCHG();
            break;
        case(0xcd):
        case(0xdd):
        case(0xed):
        case(0xfd):
            CALL(instruction, memory);
            break;
        case(0xfb):
            EI();
            break;
        case(0xfe):
            CPI(instruction);
            break;
        default:
            UnimplementedInstruction();
            break;
    }

    return;
}
void State8080::UnimplementedInstruction() {
    std::cout << "FATAL ERROR: UNIMPLEMENTED INSTRUCTION\n";
    exit(1);
}

void State8080::ADI(const unsigned char *instruction) {
    unsigned char operand = instruction[1];
    unsigned int tempAdd;
    tempAdd = operand + reg_A;
    if((tempAdd / 256) > 0)
        flagCarry = true;
    else
        flagCarry = false;
    reg_A = (unsigned char)(tempAdd % 256);

    if(reg_A == 0)
        flagZero = true;
    else
        flagZero = false;
    pc++;
    return;

}
void State8080::ANA(const unsigned char *instruction, unsigned char *memory) {
    unsigned int memAddress = reg_L;
    memAddress += reg_H * 256;
    switch (instruction[0])
    {
        case(0xa0): //B
            reg_A = reg_A & reg_B;
            break;
        case(0xa1): //C
            reg_A = reg_A & reg_C;
            break;
        case(0xa2): //D
            reg_A = reg_A & reg_D;
            break;
        case(0xa3): //E
            reg_A = reg_A & reg_E;
            break;
        case(0xa4): //H
            reg_A = reg_A & reg_H;
            break;
        case(0xa5): //L
            reg_A = reg_A & reg_L;
            break;
        case(0xa6): //M
            reg_A = reg_A & memory[memAddress];
            break;
        case(0xa7): //A
            reg_A = reg_A & reg_A;
            break;

    }

    //Set flags
    flagCarry = false;
    if(reg_A == 0)
        flagZero = true;
    else
        flagZero = false;
    return;
}
void State8080::ANI(const unsigned char *instruction) {
    unsigned char operand = instruction[1];
    flagCarry = false;
    reg_A = reg_A & operand;
    pc++; //2 byte instruction
    return;
}
void State8080::CALL(const unsigned char *instruction, unsigned char *memory) {
    unsigned char lowAddress = instruction[1];
    unsigned char highAddress = instruction[2];


    std::cout << "Stack pointer is " << sp << std::endl;
    std::cout << "Low address is " << (int)lowAddress << std::endl;
    std::cout << "High address is " << (int)highAddress << std::endl;

    //Push the program counter onto the stack
    pc += 2; //3 byte instruction
    sp--;
    memory[sp] = (pc / 256) % 256;
    sp--;
    memory[sp] = pc % 256;

    pc = lowAddress;
    pc += highAddress * 256;

    std::cout << "Program counter is: " << pc << std::endl;

    return;

}

void State8080::CPI(const unsigned char *instruction) {
    unsigned char data = instruction[1];
    if(reg_A == data)
        flagZero = true;
    else if(reg_A > data)
    {
        flagZero = false;
        flagCarry = false;
    }
    else if(reg_A < data)
    {
        flagZero = false;
        flagCarry = true;
    }

    return;
}

void State8080::DAD(const unsigned char *instruction) {
    unsigned char operand = instruction[0];
    unsigned int tempAdd;
    tempAdd = reg_L;
    tempAdd += reg_H * 256;

    switch(operand)
    {
        case(0x09): //B/C
            tempAdd += reg_C;
            tempAdd += reg_B * 256;
            break;
        case(0x19): //D/E
            tempAdd += reg_E;
            tempAdd += reg_D * 256;
            break;
        case(0x29): //H/L
            tempAdd += reg_L;
            tempAdd += reg_H * 256;
            break;
        case(0x39): //SP
            tempAdd += sp;
            break;
    }

    //Check for carry
    if((tempAdd / 65536) > 0)
        flagCarry = true;
    else
        flagCarry = false;

    //Store result
    reg_L = tempAdd % 256;
    reg_H = (int)(tempAdd / 256) % 256;

    return;
}
void State8080::DCR(const unsigned char *instruction) {
    switch(instruction[0])
    {
        case(0x05): //B
            reg_B--;
        std::cout << "B is " << (int)reg_B << std::endl;
            if(reg_B == 0x00) {
                flagZero = true;
            }
            else
                flagZero = false;
            if(reg_B == 0xFF)
                flagSign = true;
            else
                flagSign = false;
            break;
        case(0x0d): //C
            reg_C--;
            if(reg_C == 0x00)
                flagZero = true;
            else
                flagZero = false;
            if(reg_C == 0xFF)
                flagSign = true;
            else
                flagSign = false;
            break;
        case(0x15): //D
            reg_D--;
            if(reg_D == 0x00)
                flagZero = true;
            else
                flagZero = false;
            if(reg_D == 0xFF)
                flagSign = true;
            else
                flagSign = false;
            break;
        case(0x1d): //E
            reg_E--;
            if(reg_E == 0x00)
                flagZero = true;
            else
                flagZero = false;
            if(reg_E == 0xFF)
                flagSign = true;
            else
                flagSign = false;
            break;
        case(0x25): //H
            reg_H--;
            if(reg_H == 0x00)
                flagZero = true;
            else
                flagZero = false;
            if(reg_H == 0xFF)
                flagSign = true;
            else
                flagSign = false;
            break;
        case(0x2d): //L
            reg_L--;
            if(reg_L == 0x00)
                flagZero = true;
            else
                flagZero = false;
            if(reg_L == 0xFF)
                flagSign = true;
            else
                flagSign = false;
            break;
        case(0x35): //M
            //TODO: Implement memory DCR
            break;
        case(0x3d): //A
            reg_A--;
        std::cout<< "Reg A is: " << (int)reg_A << std::endl;
            if(reg_A == 0x00)
                flagZero = true;
            else
                flagZero = false;
            if(reg_A == 0xFF)
                flagSign = true;
            else
                flagSign = false;
            break;
    }
}
void State8080::EI() {
    interruptsEnabled = true;
    breakpoint = 100;
    return;
}
void State8080::INX(const unsigned char *instruction) {
    switch (instruction[0])
    {
        case(0x03): //B/C
            reg_C++;
            if(reg_C == 0x00)
                reg_B++;
            break;
        case(0x13): //D/E
            reg_E++;
            if(reg_E == 0x00)
                reg_D++;
            break;
        case(0x23): //H/L
            reg_L++;
            if(reg_L == 0x00)
                reg_H++;
            break;
        case(0x33): //SP
            sp++;
            break;
    }

    return;
}
void State8080::JMP(const unsigned char* instruction)
{
    unsigned int newPC;
    switch(instruction[0])
    {
        case(0xc2): //JNZ
            if(!flagZero)
            {
                newPC = instruction[1];
                newPC += instruction[2] * 256;
                pc = newPC;
                break;
            }
            else
            {
                pc += 2; //Continue through 3 byte instruction
                break;
            }
        case(0xc3): //JMP

            newPC = instruction[1];
            newPC += instruction[2] * 256;
            pc = newPC;
            break;
    }
    return;
}
void State8080::LDA(const unsigned char *instruction, unsigned char *memory) {
    unsigned int memAddress = instruction[1];
    memAddress += instruction[2] * 256;
    reg_A = memory[memAddress];
    pc += 2; //3 byte instruction
    return;
}
void State8080::LDAX(const unsigned char *instruction, unsigned char *memory) {

    unsigned int memAddress;
    switch(instruction[0])
    {
        case(0x0a): //B/C
            memAddress = reg_C;
            memAddress += reg_B * 256;
            reg_A = memory[memAddress];
            break;
        case(0x1a): //D/E
            memAddress = reg_E;
            memAddress += reg_D * 256;
            reg_A = memory[memAddress];
            break;
    }

    return;
}
void State8080::LXI(const unsigned char *instruction) {
    unsigned char targetRegPair = instruction[0]; //Register pair to write to
    unsigned char lowData = instruction[1];
    unsigned char highData = instruction[2];

    switch(targetRegPair)
    {
        case(0x01): //B/C
            reg_B = highData;
            reg_C = lowData;
            break;
        case(0x11): //D/E
            reg_D = highData;
            reg_E = lowData;
            break;
        case(0x21): //H/L
            reg_H = highData;
            reg_L = lowData;
            break;
        case(0x31): //SP
            sp = lowData;
            sp += highData * 256;
            break;
    }

    pc += 2; //3 byte instruction
    return;
}

void State8080::MOV(const unsigned char *instruction, unsigned char *memory)
{
    unsigned char transfer = 0; //The data being transferred
    unsigned int memAddress = 0; //Memory address to read/write to
    unsigned short operand1 = (*instruction / 8) % 8;
    unsigned short operand2 = *instruction % 8;
    if(operand1 == 6) //reg1 == M, move to memory
    {
        memAddress = reg_H * 256;
        memAddress += reg_L;

        switch(operand2)
        {
            case(0)://B
                memory[memAddress] = reg_B;
                break;
            case(1)://C
                memory[memAddress] = reg_C;
                break;
            case(2)://D
                memory[memAddress] = reg_D;
                break;
            case(3)://E
                memory[memAddress] = reg_E;
                break;
            case(4)://H
                memory[memAddress] = reg_H;
                break;
            case(5)://L
                memory[memAddress] = reg_L;
                break;
            case(7)://A
                memory[memAddress] = reg_A;
                break;
        }
    }
    else if(operand2 == 6) //reg2 == M, move from memory
    {
        memAddress = reg_H * 256;
        memAddress += reg_L;

        switch(operand1)
        {
            case(0)://B
                reg_B = memory[memAddress];
                break;
            case(1)://C
                reg_C = memory[memAddress];
                break;
            case(2)://D
                reg_D = memory[memAddress];
                break;
            case(3)://E
                reg_E = memory[memAddress];
                break;
            case(4)://H
                reg_H = memory[memAddress];
                break;
            case(5)://L
                reg_L = memory[memAddress];
                break;
            case(7)://A
                reg_A = memory[memAddress];
                break;
        }
    }
    else //Register to register copy
    {
        switch(operand2) //Source
        {
            case(0)://B
                transfer = reg_B;
                break;
            case(1)://C
                transfer = reg_C;
                break;
            case(2)://D
                transfer = reg_D;
                break;
            case(3)://E
                transfer = reg_E;
                break;
            case(4)://H
                transfer = reg_H;
                break;
            case(5)://L
                transfer = reg_L;
                break;
            case(7)://A
                transfer = reg_A;
                break;
        }
        switch(operand1) //Destination
        {
            case(0)://B
                reg_B = transfer;
                break;
            case(1)://C
                reg_C = transfer;
                break;
            case(2)://D
                reg_D = transfer;
                break;
            case(3)://E
                reg_E = transfer;
                break;
            case(4)://H
                reg_H = transfer;
                break;
            case(5)://L
                reg_L = transfer;
                break;
            case(7):
                reg_A = transfer;
                break;
        }

    }

    return;
}

void State8080::MVI(const unsigned char *instruction, unsigned char *memory) {
    unsigned char dest = instruction[0];
    unsigned char data = instruction[1];
    unsigned int memAddress;
    switch(dest)
    {
        case(0x06): //B
            reg_B = data;
            break;
        case(0x0e): //C
            reg_C = data;
            break;
        case(0x16): //D
            reg_D = data;
            break;
        case(0x1e): //E
            reg_E = data;
            break;
        case(0x26): //H
            reg_H = data;
            break;
        case(0x2e): //L
            reg_L = data;
            break;
        case(0x36): //M
            memAddress = reg_L;
            memAddress += reg_H * (unsigned int)256;
            memory[memAddress] = data;
            break;
        case(0x3e): //A
            reg_A = data;
            break;
    }

    pc++; //2 byte instruction
    return;
}

void State8080::OUT(const unsigned char *instruction) {
    pc++; //2 byte instruction
    std::cout << "Outputting to " << (int)instruction[1] << std::endl;
    return;
}
void State8080::POP(const unsigned char *instruction, unsigned char *memory) {
    switch(instruction[0])
    {
        case (0xc1): //B/C
            reg_C = memory[sp];
            sp++;
            reg_B = memory[sp];
            sp++;
            break;
        case (0xd1): //D/E
            reg_E = memory[sp];
            sp++;
            reg_D = memory[sp];
            sp++;
            break;
        case (0xe1): //H/L
            reg_L = memory[sp];
            sp++;
            reg_H = memory[sp];
            sp++;
            break;
        case (0xf1): //PSW //TODO: flag -> byte conversion
            break;
    }

}
void State8080::PUSH(const unsigned char *instruction, unsigned char *memory) {
    unsigned char operand = instruction[0];
    switch (operand)
    {
        case(0xc5): //B/C
            sp--;
            memory[sp] = reg_B;
            sp--;
            memory[sp] = reg_C;
            break;
        case(0xd5): //D/E
            sp--;
            memory[sp] = reg_D;
            sp--;
            memory[sp] = reg_E;
            break;
        case(0xe5): //H/L
            sp--;
            memory[sp] = reg_H;
            sp--;
            memory[sp] = reg_L;
            break;
        case(0xf5): //TODO: Properly convert flags to byte
            break;
    }
}

void State8080::RET(unsigned char *memory) {
    pc = memory[sp];
    sp++;
    pc += memory[sp] * 256;
    sp++;
    return;
}
void State8080::RRC() {
    flagCarry = (bool)(reg_A % 2);
    reg_A /= 2;

    reg_A += (unsigned char)flagCarry * 128;

    return;
}
void State8080::STA(const unsigned char *instruction, unsigned char *memory) {
    unsigned int memAddress = instruction[1];
    memAddress += instruction[2] * 256;
    memory[memAddress] = reg_A;
    pc += 2; //3byte instruction
    return;
}

void State8080::XCHG() {
    unsigned char tempCopy;
    tempCopy = reg_H;
    reg_H = reg_D;
    reg_D = tempCopy;

    tempCopy = reg_L;
    reg_L = reg_E;
    reg_E = tempCopy;

    return;

}

void State8080::XRA(const unsigned char *instruction, unsigned char *memory) {
    unsigned int memAddress = reg_L;
    memAddress += reg_H * 256;

    switch(instruction[0])
    {
        case(0xa8)://B
            reg_A = reg_A ^ reg_B;
            break;
        case(0xa9)://C
            reg_A = reg_A ^ reg_C;
            break;
        case(0xaa)://D
            reg_A = reg_A ^ reg_D;
            break;
        case(0xab)://E
            reg_A = reg_A ^ reg_E;
            break;
        case(0xac)://H
            reg_A = reg_A ^ reg_H;
            break;
        case(0xad)://L
            reg_A = reg_A ^ reg_L;
            break;
        case(0xae)://M
            reg_A = reg_A ^ memory[memAddress];
            break;
        case(0xaf)://A
            reg_A = reg_A ^ reg_A;
            break;
    }

    //Set flags
    flagCarry = false;
    flagAuxCarry = false;

    if(reg_A == 0)
        flagZero = true;
    else
        flagZero = false;

    return;

}

