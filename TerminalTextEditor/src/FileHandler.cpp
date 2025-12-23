#include "C:\Users\15142\CLionProjects\TerminalTextEditor\include\FileHandler.h"
#include <fstream>
#include <iostream>

bool FileHandler::openFile(const std::string& path,TextBuffer& buffer) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "无法打开文件:" << path << std::endl;
        return false;
    }
    buffer = TextBuffer();
    std::string line;
    while (getline(file,line)) {
        buffer.insertNewLine();
        buffer.insertString(line);
    }
    file.close();
    return true;
}
bool FileHandler::saveFile(const std::string& path,const TextBuffer& buffer) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "无法保存文件:" << path << std::endl;
        return false;
    }
    const auto& lines = buffer.getLines();
    for (const auto& line : lines) {
        file << line << "\n";
    }
    file.close();
    return true;
}