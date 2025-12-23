#pragma once
#include <vector>
#include <string>
class TextBuffer {
private:
    std::vector<std::string> lines;//存每行文本
    int cursor_row = 0;//行光标
    int cursor_col = 0;//列光标
void validateCursor();
public:
TextBuffer() {lines.emplace_back("");}
    //光标操作
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    int  getCursorRow() const {return cursor_row;}
    int  getCursorCol() const {return cursor_col;}
    //文本编辑
    void moveCursorTo(int row,int col);
    void insertChar(char c);
    void deleteChar();
    void insertNewLine();
    void insertString(const std::string& s);
    void deleteLine();
    //文本获取
    const  std::vector<std::string>& getLines() const {return lines;}
    int getCount() const {return lines.size();}
    const std::string getLine(int row) const {return lines[row];};
};