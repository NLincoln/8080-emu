#include "8080Core.h"

std::string State8080::GetOpcode(unsigned char opcode)
{
    return OPCODES[opcode];
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

void State8080::UnimplementedInstruction() {
    std::cout << "FATAL ERROR: UNIMPLEMENTED INSTRUCTION\n";
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

