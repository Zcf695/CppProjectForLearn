#include  "C:\Users\15142\CLionProjects\TerminalTextEditor\include\TextBuffer.h"
void TextBuffer::validateCursor() {
    if (cursor_row < 0) cursor_row = 0;
    if (cursor_row >= lines.size()) cursor_row = lines.size() - 1;
    if (cursor_col < 0) cursor_col = 0;
    if (cursor_col >= lines[cursor_row].size()) cursor_col = lines[cursor_row].size();
}
void TextBuffer::moveCursorUp() {
    cursor_row--;
    validateCursor();
  if (cursor_col > lines[cursor_row].size()) cursor_col = lines[cursor_row].size();
}
void TextBuffer::moveCursorDown() {
    cursor_row++;
    validateCursor();
    if (cursor_col > lines[cursor_row].size()) cursor_col = lines[cursor_row].size();
}
void TextBuffer::moveCursorLeft() {
    cursor_col--;
    validateCursor();
}
void TextBuffer::moveCursorRight() {
    cursor_col++;
    validateCursor();
}
void TextBuffer::moveCursorTo(int row,int col) {
    cursor_col = col;
    cursor_row = row;
    validateCursor();
}

void TextBuffer::insertChar(char c) {
    lines[cursor_row].insert(cursor_col,1,c);
    cursor_col++;
}
void TextBuffer::deleteChar() {
if (cursor_col == 0) return;
    lines[cursor_row].erase(cursor_col - 1,1);
    cursor_col--;
}
void TextBuffer::insertNewLine() {
    std::string current_line = lines[cursor_row];
    std::string left = current_line.substr(0, cursor_col);
    std::string right = current_line.substr(cursor_col);
    lines[cursor_row] = left;
    if (!right.empty()) {
        lines.insert(lines.begin() + cursor_row + 1, right);
    } else {
        lines.insert(lines.begin() + cursor_row + 1, "");
    }

    cursor_row++;
    cursor_col = 0;
    validateCursor();
}
void TextBuffer::insertString(const std::string& s) {
    for (char c : s) insertChar(c);
}
void TextBuffer::deleteLine(){
    if (lines.size() == 1) {
        lines[0].clear();
        cursor_col = 0;
        return;
    }
    if (lines[cursor_row].empty()) {
        lines.erase(lines.begin() + cursor_row);
        if (cursor_row >= lines.size()) {
            cursor_row = lines.size() - 1;
        }
        validateCursor();
        return;
    }
    std::string current_line = lines[cursor_row];
    lines.erase(lines.begin() + cursor_row);
    if (cursor_row >= lines.size()) {
        cursor_row = lines.size() - 1;
    }
    validateCursor();
}
