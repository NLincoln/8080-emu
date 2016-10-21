#include "8080Core.h"

std::string State8080::GetOpcode(unsigned char opcode)
{
    return OPCODES[opcode];
}

State8080::State8080()
{
    instruction = new unsigned char[3];
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
}
void State8080::RunInstruction(const unsigned char *data, unsigned char *memory) {

    //Fill instruction buffer
    instruction[0] = data[pc];
    instruction[1] = data[pc + 1];
    instruction[2] = data[pc + 2];

    //Increment pc before instruction in case of JMP
    pc++;

    std::cout<<GetOpcode(instruction[0]);

    //Go to the proper instruction
    switch(instruction[0])
    {
        case(0x00):
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
        case(0xc3):
            JMP(instruction);
            break;
        default:
            UnimplementedInstruction();
            break;
    }

    return;
}
void State8080::UnimplementedInstruction() {
    std::cout << "FATAL ERROR: UNIMPLEMENTED INSTRUCTION\n";
    std::string test;
    std::cin>>test;
    exit(1);
}

void State8080::JMP(const unsigned char* instruction)
{
    unsigned int newPC = 0;
    newPC += instruction[1];
    newPC += instruction[2] * 256;
    pc = newPC;
    return;
}

void State8080::MOV(const unsigned char *instruction, unsigned char *memory)
{
    unsigned char transfer = 0; //The data being transferred
    unsigned int memAddress = 0; //Memory address to read/write to
    unsigned short operand1 = (*instruction & 0b00111000) / 8;
    unsigned short operand2 = *instruction & 0b00000111;
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
}

