#include "C:\Users\15142\CLionProjects\TerminalTextEditor\include\TextBuffer.h"
#include "C:\Users\15142\CLionProjects\TerminalTextEditor\include\Terminal.h"
#include "C:\Users\15142\CLionProjects\TerminalTextEditor\include\CommandManager.h"
#include "C:\Users\15142\CLionProjects\TerminalTextEditor\include\FileHandler.h"
#include <iostream>
#if defined(__linux__) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
void setRawMode(bool enable) {
    static termios old_attr;  // 静态变量：保存原始终端属性，只初始化一次
    if (enable) {
        tcgetattr(STDIN_FILENO, &old_attr);  // 获取当前终端属性
        termios new_attr = old_attr;
        // ICANON：关闭行缓冲；ECHO：关闭回显
        new_attr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);  // 立即应用新属性
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);  // 恢复原始属性
    }
}
#elif _WIN32  // Windows系统
#include <windows.h>
void setRawMode(bool enable) {
    static HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    static DWORD old_mode;
    if (enable) {
        GetConsoleMode(hStdin, &old_mode);
        SetConsoleMode(hStdin, 0);  // 关闭所有模式（回显、行缓冲）
    } else {
        SetConsoleMode(hStdin, old_mode);
    }
}
#endif

int main() {
    // 初始化核心对象
    TextBuffer buffer;          // 文本缓冲区（存储编辑的内容）
    CommandManager cmd_mgr;     // 命令管理器（处理撤销/重做）
    Terminal::hideCursor();     // 隐藏终端默认光标，用自定义光标
    setRawMode(true);           // 进入原始模式，实现实时输入响应

    // 主循环：程序运行的核心，直到用户退出
    bool running = true;
    while (running) {
        Terminal::drawBuffer(buffer);  // 每次循环刷新界面，显示最新文本

        // 读取用户输入（原始模式下，按一个键立即读取）
        char c = std::cin.get();
        if (c == '\033') {  // 功能键（方向键、F键等）以\033开头
            std::cin.get();  // 读取功能键的第二个字符（固定为'['）
            char dir = std::cin.get();  // 读取方向键标识（A=上，B=下，C=右，D=左）
            switch (dir) {
                case 'W': buffer.moveCursorUp(); break;
                case 'S': buffer.moveCursorDown(); break;
                case 'D': buffer.moveCursorRight(); break;
                case 'A': buffer.moveCursorLeft(); break;
            }
        } else if (c == 127 || c == '\b') {  // 退格键（不同系统编码不同）
            int row = buffer.getCursorRow();
            int col = buffer.getCursorCol();
            if (col > 0) {  // 行首不能删除
                // 获取要删除的字符（用于撤销时恢复）
                char deleted_char = buffer.getLine(row)[col-1];
                // 创建删除命令，交给命令管理器执行
                cmd_mgr.executeCommand(buffer, new DeleteCharCommand(deleted_char, row, col));
            }
        } else if (c == '\n' || c == '\r') {  // 兼容Windows的\r和Linux的\n
            int row = buffer.getCursorRow();
            int col = buffer.getCursorCol();
            // 执行换行命令（核心：通过命令管理器封装）
            cmd_mgr.executeCommand(buffer, new InsertNewLineCommand(row, col));

        } else if (c == 19) {  // Ctrl+S（ASCII码19）：保存文件
            FileHandler::saveFile("editor.txt", buffer);
        } else if (c == 26) {  // Ctrl+Z（ASCII码26）：撤销
            cmd_mgr.undo(buffer);
        } else if (c == 25) {  // Ctrl+Y（ASCII码25）：重做
            cmd_mgr.redo(buffer);
        } else if (c == 17) {  // Ctrl+Q（ASCII码17）：退出程序
            running = false;
        } else if (isprint(c)) {  // 可打印字符（字母、数字、符号等）
            int row = buffer.getCursorRow();
            int col = buffer.getCursorCol();
            // 创建插入命令，交给命令管理器执行
            cmd_mgr.executeCommand(buffer, new InsertCharCommand(c, row, col));
        }
    }

    // 程序退出前恢复终端设置，避免终端异常
    setRawMode(false);
    Terminal::showCursor();
    Terminal::clearScreen();
    return 0;
}
