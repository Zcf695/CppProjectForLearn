#pragma once
#include <stack>
#include "TextBuffer.h"
class Command {
public:
    virtual ~Command() = default;
    virtual void execute(TextBuffer& buffer) = 0;
    virtual void undo(TextBuffer& buffer) = 0;
};
//封装插入
class InsertCharCommand:public Command {
    private:
    char c;
    int row,col;
    public:
    InsertCharCommand(char c,int row,int col):c(c),row(row),col(col){}
    void execute(TextBuffer& buffer) override;
    void undo(TextBuffer& buffer) override;
};
class InsertNewLineCommand : public Command {
private:
    int row, col;
public:
    InsertNewLineCommand(int row, int col) : row(row), col(col) {}
    void execute(TextBuffer& buffer) override;
    void undo(TextBuffer& buffer) override;
};
class DeleteCharCommand : public Command {
private:
    char c;
    int row,col;
public:
    DeleteCharCommand(char c, int row, int col) : c(c), row(row), col(col) {}
    void execute(TextBuffer& buffer) override;
    void undo(TextBuffer& buffer) override;
};
class CommandManager {
private:
    std::stack<Command*> undo_stack;//撤销ctrl+z
    std::stack<Command*> redo_stack;//重做ctrl+y
public:
    ~CommandManager();
    void executeCommand(TextBuffer& buffer,Command* cmd);
    void undo(TextBuffer& buffer);
    void redo(TextBuffer& buffer);
};