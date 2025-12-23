#include "C:\Users\15142\CLionProjects\TerminalTextEditor\include\CommandManager.h"
void InsertCharCommand::execute(TextBuffer& buffer) {
    buffer.moveCursorTo(row, col);
    buffer.insertChar(c);
}
void InsertCharCommand::undo(TextBuffer& buffer) {
    buffer.moveCursorTo(row,col);
    buffer.insertChar(c);
}
void DeleteCharCommand::execute(TextBuffer& buffer) {
    buffer.moveCursorTo(row, col);
    buffer.deleteChar();
}

void DeleteCharCommand::undo(TextBuffer& buffer) {
    buffer.moveCursorTo(row, col);
    buffer.insertChar(c);
}
void InsertNewLineCommand::execute(TextBuffer& buffer) {
    buffer.moveCursorTo(row, col);  // 定位到换行前位置
    buffer.insertNewLine();         // 执行换行
}
void InsertNewLineCommand::undo(TextBuffer& buffer) {
    // 撤销换行：将当前行（新行）的内容合并回上一行，删除当前行
    int current_row = row + 1;  // 换行后的行号
    std::string current_line = buffer.getLine(current_row);  // 新行内容
    buffer.moveCursorTo(row, col);  // 回到原位置
    buffer.insertString(current_line);  // 合并新行内容到原行
    buffer.deleteLine();  // 删除新行
}

void CommandManager::executeCommand(TextBuffer& buffer,Command* cmd) {
    cmd->execute(buffer);//执行命令
    undo_stack.push(cmd);
    while (!redo_stack.empty()) {
        delete redo_stack.top();
        redo_stack.pop();
    }
}
void CommandManager::undo(TextBuffer& buffer) {
    if (undo_stack.empty()) return;
    Command* cmd = undo_stack.top();
    undo_stack.pop();
    cmd->undo(buffer);
    redo_stack.push(cmd);
}
void CommandManager::redo(TextBuffer& buffer) {
    if (redo_stack.empty()) return;
    Command* cmd = redo_stack.top();
    redo_stack.pop();
    cmd->execute(buffer);
    undo_stack.push(cmd);
}

CommandManager::~CommandManager() {
    while (!undo_stack.empty()) {
        delete undo_stack.top();
        undo_stack.pop();
    }
    while (!redo_stack.empty()) {
        delete redo_stack.top();
        redo_stack.pop();
    }
}
