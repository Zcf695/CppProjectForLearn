cmake_minimum_required(VERSION 3.20)
project(TerminalTextEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(TerminalTextEditor main.cpp
        include/TextBuffer.h
        include/Terminal.h
        include/CommandManager.h
        include/FileHandler.h
        src/TextBuffer.cpp
        src/Terminal.cpp
        src/CommandManager.cpp
        src/FileHandler.cpp
        )

include_directories(include)