#include "8080Core.h"

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
        case(0x0c):
        case(0x15):
        case(0x1c):
        case(0x25):
        case(0x2c):
        case(0x35):
        case(0x3c):
            DCR(instruction);
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
        case(0xc2):
        case(0xc3):
            JMP(instruction);
            break;
        case(0xc9):
        case(0xd9):
            RET(memory);
            break;
        case(0xcd):
        case(0xdd):
        case(0xed):
        case(0xfd):
            CALL(instruction, memory);
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

void State8080::CALL(const unsigned char *instruction, unsigned char *memory) {
    unsigned char lowAddress = instruction[1];
    unsigned char highAddress = instruction[2];


    std::cout << "Stack pointer is " << sp << std::endl;
    std::cout << "Low address is " << (int)lowAddress << std::endl;
    std::cout << "High address is " << (int)highAddress << std::endl;

    //Push the program counter onto the stack
    pc += 2; //3 byte instruction
    memory[sp] = pc % 256;
    sp--;
    memory[sp] = (pc / 256) % 256;
    sp--;

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

void State8080::DCR(const unsigned char *instruction) {
    switch(instruction[0])
    {
        case(0x05): //B
            reg_B--;
        std::cout << "B is " << (int)reg_B << std::endl;
            if(reg_B == 0x00) {
                flagZero = true;
            }
            if(reg_B == 0xFF)
                flagSign = true;
            break;
        case(0x0c): //C
            reg_C--;
            if(reg_C == 0x00)
                flagZero = true;
            if(reg_C == 0xFF)
                flagSign = true;
            break;
        case(0x15): //D
            reg_D--;
            if(reg_D == 0x00)
                flagZero = true;
            if(reg_D == 0xFF)
                flagSign = true;
            break;
        case(0x1c): //E
            reg_E--;
            if(reg_E == 0x00)
                flagZero = true;
            if(reg_E == 0xFF)
                flagSign = true;
            break;
        case(0x25): //H
            reg_H--;
            if(reg_H == 0x00)
                flagZero = true;
            if(reg_H == 0xFF)
                flagSign = true;
            break;
        case(0x2c): //L
            reg_L--;
            if(reg_L == 0x00)
                flagZero = true;
            if(reg_L == 0xFF)
                flagSign = true;
            break;
        case(0x35): //M
            //TODO: Implement memory DCR
            break;
        case(0x3c): //A
            reg_A--;
            if(reg_A == 0x00)
                flagZero = true;
            if(reg_A == 0xFF)
                flagSign = true;
            break;
    }
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
            reg_B = lowData;
            reg_C = highData;
            break;
        case(0x11): //D/E
            reg_D = lowData;
            reg_E = highData;
            break;
        case(0x21): //H/L
            reg_H = lowData;
            reg_L = highData;
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
            memAddress = reg_H * (unsigned int)256;
            memory[memAddress] = data;
            break;
        case(0x3e): //A
            reg_A = data;
            break;
    }

    pc++; //2 byte instruction
    return;
}

void State8080::RET(unsigned char *memory) {
    sp++;
    pc = memory[sp] * 256;
    sp++;

    pc += memory[sp];
}

