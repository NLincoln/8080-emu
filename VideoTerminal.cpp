//
// Created by joseph on 10/23/16.
//

#include "VideoSystem.h"
#include <iostream>
#include <cmath>

void TerminalOutput::ClearScreen() {
    for(int i = 0; i < 224; i++)
    {
        std::cout << "\n";
    }
    return;
}

void TerminalOutput::DrawByte(const unsigned char byte) {
    bool pixel = false;
    unsigned char remainingByte;


    for(int i = 0; i < 8; i++)
    {
        remainingByte = (unsigned char)(byte / exp2(i));
        pixel = (bool)(remainingByte % 2);
        if(pixel)
            std::cout << "1";
        else
            std::cout << ".";
    }


    return;
}

void TerminalOutput::DrawScreen(const unsigned char *memory, const unsigned int vramSize, const int rowSize) {
    unsigned int rowPos = 0;
    unsigned int screenPos = 0;
    unsigned int row = 0;
    unsigned int vramAddress = VRAM_OFFSET;

    while(screenPos < vramSize)
    {
        while(rowPos < 224)
        {
            vramAddress += 1;
            DrawByte(memory[vramAddress]);
            screenPos += 1;
            rowPos += 8;
        }
        std::cout << "\n";
        row++;
        std::cout << row;
        rowPos = 0;
    }

    return;
}
