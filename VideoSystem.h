//
// Created by joseph on 10/23/16.
//

#ifndef INC_8080EMULATOR_VIDEOSYSTEM_H
#define INC_8080EMULATOR_VIDEOSYSTEM_H

const unsigned int VRAM_OFFSET = 9216;

//Provides a terminal representation of the game's output
class TerminalOutput
{
private:
    //Outputs 8 1bpp "pixels" onto the terminal inside the
    void DrawByte(const unsigned char byte);
public:
    //Clears the terminal
    void ClearScreen();

    //Draws to the terminal using the contents of memory starting at VRAM_OFFSET
    //for the number of bytes specified by vramSize
    //rowSize: The number of pixels per row
    void DrawScreen(const unsigned char *memory, const unsigned int vramSize, const int rowSize);

};
#endif //INC_8080EMULATOR_VIDEOSYSTEM_H
