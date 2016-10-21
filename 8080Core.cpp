#include "8080Core.h"

std::string GetOpcode(unsigned char opcode)
{
    return OPCODES[opcode];
}

void Init8080(State8080* cpuState)
{
    cpuState->reg_A = 0;
    cpuState->reg_B = 0;
    cpuState->reg_C = 0;
    cpuState->reg_D = 0;
    cpuState->reg_E = 0;
    cpuState->reg_H = 0;
    cpuState->reg_L = 0;
    cpuState->reg_PSW = 0;
    cpuState->pc = 0;
}

void JMP(const unsigned char* instruction, State8080* cpuState)
{
    unsigned int newPC = 0;
    newPC += instruction[1];
    newPC += instruction[2] * 16;

    cpuState->pc = newPC;
    return;
}

void MOV(const unsigned char &instruction, State8080* cpuState)
{
    unsigned char transfer = 0; //The data being transferred
    unsigned short operand1 = (instruction & 0b00111000) / 8;
    unsigned short operand2 = instruction & 0b00000111;
    if(operand1 == 6) //reg1 == M, move to memory
    {

    }
    else if(operand2 == 6) //reg2 == M, move from memory
    {

    }
    else //Register to register copy
    {
        switch(operand2) //Source
        {
            case(0)://B
                transfer = cpuState->reg_B;
                break;
            case(1)://C
                transfer = cpuState->reg_C;
                break;
            case(2)://D
                transfer = cpuState->reg_D;
                break;
            case(3)://E
                transfer = cpuState->reg_E;
                break;
            case(4)://H
                transfer = cpuState->reg_H;
                break;
            case(5)://L
                transfer = cpuState->reg_L;
                break;
            case(7)://A
                transfer = cpuState->reg_A;
                break;
        }
        switch(operand1) //Destination
        {
            case(0)://B
                cpuState->reg_B = transfer;
                break;
            case(1)://C
                cpuState->reg_C = transfer;
                break;
            case(2)://D
                cpuState->reg_D = transfer;
                break;
            case(3)://E
                cpuState->reg_E = transfer;
                break;
            case(4)://H
                cpuState->reg_H = transfer;
                break;
            case(5)://L
                cpuState->reg_L = transfer;
                break;
            case(7):
                cpuState->reg_A = transfer;
                break;
        }

    }
}

