

#include "8080Core.h"

int Disassemble(unsigned char* buffer, int pc)
{
    unsigned char opcode = buffer[pc];
    std::cout<<OPCODES[opcode];
    std::cout<<"\n";
    return 1;
}

