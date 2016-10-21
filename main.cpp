//main.cpp
//Contains code for getting files and running the emulator

#include <fstream>
#include <vector>

#include "8080Core.h"

int main()
{
    std::ifstream file("invaders.h", std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cerr << "Error opening invaders.h file " << std::endl;
        exit(1);
    }
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer((unsigned long) file_size);
    if (!file.read((char*)&buffer[0], file_size))
    {
        std::cerr << "Error reading in space invaders file" << std::endl;
        exit(2);
    }

    unsigned int programCounter = 0;
    std::cout << "Beginning Program" << std::endl;
    while(programCounter < file_size)
    {
        std::cout << GetOpcode(buffer[programCounter]) << std::endl;
        programCounter += 1;
    }

    file.close();

    return 0;
}