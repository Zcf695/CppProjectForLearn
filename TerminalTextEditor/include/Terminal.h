#pragma once
#include "TextBuffer.h"
#include<string>
class Terminal {
public:
    static void clearScreen();
    static void moveCursor(int row,int col);
    static void hideCursor();
    static void showCursor();
    static void drawBuffer(const TextBuffer& buffer);
};




