#pragma once
#include "TextBuffer.h"
#include <string>

class FileHandler {
public:
    static bool openFile(const std::string& path,TextBuffer& buffer);
    static bool saveFile(const std::string& path,const TextBuffer& buffer);
};