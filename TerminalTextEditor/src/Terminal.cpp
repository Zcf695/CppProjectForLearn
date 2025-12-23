//
// Created by 15142 on 2025/12/10.
//
#include"C:\Users\15142\CLionProjects\TerminalTextEditor\include\Terminal.h"
#include<iostream>
void Terminal::clearScreen() {
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}
void Terminal::moveCursor(int row,int col) {
    std::cout << "\033[" << (row+1) << ";" << (col + 5) << "H";
    std::cout.flush();
}

void Terminal::hideCursor() {
    std::cout << "\033[?251";
    std::cout.flush();
}
void Terminal::showCursor() {
    std::cout << "\033[?25h";
    std::cout.flush();

}
void Terminal::drawBuffer(const TextBuffer& buffer) {
    clearScreen();
    const auto& lines = buffer.getLines();
    int max_line_num = lines.size();
    int num_width = std::to_string(max_line_num).size();

    for (int i = 0; i < lines.size(); i++) {
        std::cout << std::string(num_width - std::to_string(i+1).size(), ' ')
                  << (i+1) << ": " << lines[i] << "\n";
    }

    moveCursor(buffer.getCursorRow(), buffer.getCursorCol());
    std::cout.flush();
}