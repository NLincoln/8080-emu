//main.cpp
//Contains code for getting files and running the emulator

#include <fstream>
#include <vector>

#include "8080Core.h"
#include "VideoSystem.h"

int main()
{
    unsigned char* memory = new unsigned char[0x3fff];
    std::string fileName;

    std::cout << "Loading File\n";
    std::cout << "Please enter filename for emulation\n";
    std::cin>>fileName;

    std::fstream file(fileName, std::fstream::binary | std::fstream::in);
    file.seekg(0, std::ios_base::end);
    long fsize = file.tellg();
    file.seekg(0, std::ios_base::beg);

    file.read((char*)memory, fsize);
    file.close();
    State8080* cpu = new State8080(fsize);
    cpu->InitCPU();

    VideoSDL screen;
    screen.InitScreen();
    unsigned int programCounter = 0;
    std::cout << "Beginning Program" << std::endl;
    while(true)
    {
        cpu->RunInstruction(memory);
        programCounter += 1;
        if(cpu->breakpoint == 0)
            break;
        if(cpu->breakpoint > 0)
            cpu->breakpoint--;
    }



    screen.DrawFrame(memory, 7168);

    std::cin>>fileName; //Wait for input before shutting down
    screen.Shutdown();



    return 0;
}