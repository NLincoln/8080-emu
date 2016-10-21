#include "8080Core.h"

int Disassemble(unsigned char opcode)
{
    std::cout << OPCODES[opcode] << std::endl;
    return 1;
}

