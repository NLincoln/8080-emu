//main.cpp
//Contains code for getting files and running the emulator

#include <fstream>
#include <vector>

#include "8080Core.h"

int main()
{
    State8080* cpu = new State8080();
    unsigned char* mainRam = (unsigned char*)malloc(0x2000);
    std::string fileName;

    std::cout << "Loading File\n";
    std::cout << "Please enter filename for emulation\n";
    std::cin>>fileName;

    std::fstream file(fileName, std::fstream::binary | std::fstream::in);
    file.seekg(0, std::ios_base::end);
    int fsize = file.tellg();
    file.seekg(0, std::ios_base::beg);

    unsigned char* buffer = (unsigned char*)malloc(fsize);
    file.read((char*)buffer, fsize);
    cpu->InitCPU();

    unsigned int programCounter = 0;
    std::cout << "Beginning Program" << std::endl;
    for(int i = 0; i < 10; i++)
    {
        cpu->RunInstruction(buffer, mainRam);
        programCounter += 1;
    }

    file.close();
    delete(cpu);
    delete(buffer);
    delete(mainRam);

    std::cin>>fileName;

    return 0;
}