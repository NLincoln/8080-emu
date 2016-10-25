//
// Created by joseph on 10/24/16.
//

#include "VideoSystem.h"
#include <iostream>
#include <cmath>
void VideoSDL::InitScreen() {

    std::cout << "Initializing SDL..." << std::endl;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(256, 224, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &mainWindow, &gameRenderer);

    return;

}

void VideoSDL::DrawByte(const unsigned char byte, unsigned int xPos, unsigned int yPos) {
    unsigned int x = xPos;
    unsigned int y = yPos;
    bool pixelOn;
    unsigned char remainingByte;

    for(int i = 0; i < 8; i++) {
        remainingByte = (unsigned char) (byte / exp2(i));
        pixelOn = (bool) (remainingByte % 2);
        DrawPixel(pixelOn, x, y);
        x++;
    }
    return;
}

void VideoSDL::DrawPixel(const bool on, unsigned int xPos, unsigned int yPos) {
    if(on)
        SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);

    SDL_RenderDrawPoint(gameRenderer, xPos, yPos);

    return;
}

void VideoSDL::DrawFrame(const unsigned char *memory, const unsigned int vramSize) {
    unsigned int rowPos = 0;
    unsigned int screenPos = 0;
    unsigned int row = 0;
    unsigned int vramAddress = VRAM_OFFSET;

    while(screenPos < vramSize)
    {
        while(rowPos < 256)
        {
            DrawByte(memory[vramAddress], rowPos, row);
            vramAddress += 1;
            screenPos += 1;
            rowPos += 8;
        }
        row++;
        rowPos = 0;
    }

    SDL_RenderPresent(gameRenderer);

    return;

}

void VideoSDL::Shutdown() {
    SDL_Quit();
    return;
}

