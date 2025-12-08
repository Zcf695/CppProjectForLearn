#include<iostream>
#include<string>
#include<stack>
#include<vector>
#include<cmath>
#include<stdexcept>
#include<cctype>
#include<algorithm>
#include<iomanip>
using namespace std;
enum class TokenType {
    NUMBER, //数字
    PLUS, //+
    MINUS, //-
    MULTIPLY, //*
    DIVIDE, // /
    LPAREN, // (
    RPAREN, // )
    SIN, //sin函数
    COS, //cos函数
    END
};//创建枚举类，避免冲突
struct Token {
    TokenType type;
    double value;
    Token(TokenType t,double v = 0.0):type(t),value(v){}//构造函数
};
//词法分析器（Tokenizer类）
class Tokenizer {
private:
    string expr;//表达式
    size_t pos;//记录位置
    //跳过字符串空格部分,私有函数
    void skipWhitespace() {
        while (pos < expr.size() && isspace(expr[pos])) pos++;
    }
    //解析数字,私有函数
    double parseNumber() {
        size_t start = pos;
        //匹配小数,整数
        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) pos++;
        //匹配科学计数
        if (pos < expr.size() && (expr[pos] == 'e' || expr[pos] == 'E') ) {
            pos++;
            //跳过+，-
            if (pos < expr.size() && (expr[pos] == '+' || expr[pos] == '-')) {
                pos++;
            }
            //检查科学计数法格式e后必须跟数字
            if (pos >=expr.size() || !isdigit(expr[pos])) {
                throw invalid_argument("科学计数法格式错误，正确实例（1e3，2.5e-4）");
            }
            //匹配数字
            while (pos < expr.size() && isdigit(expr[pos])) {
                pos++;
            }
        }
        //截取的字符串转double
        try {
            return stod(expr.substr(start,pos - start));
        }catch (...) {
            throw invalid_argument("数字格式错误"+expr.substr(start,pos - start));
        }
    }
    //解析函数sin，cos（支持大小写，如SIN、Cos、sin），私有函数
    TokenType parseFunction() {
        size_t start = pos;
        //匹配字母
        while (pos < expr.size() && isalpha(expr[pos])) pos++;
        string func = expr.substr(start, pos - start);
        transform(func.begin(), func.end(), func.begin(), ::tolower);
        if (func == "sin") return TokenType::SIN;
        if (func == "cos") return TokenType::COS;
        throw invalid_argument("未知函数：" + func + "当前仅支持sin/cos");
    }
public:
    //构造函数：初始化表达试和解析位置
    //const& 引用传递。const保证不修改字符串
    Tokenizer(const string& e):expr(e),pos(0){}
    //公有核心函数，对外接口
    Token nextToken() {
        skipWhitespace();
        //解析到字符串末尾
        if (pos >= expr.size() ) return Token(TokenType::END);
        char c = expr[pos];//获取当前字符
        //情况1：解析数字
        if (isdigit(c) || c == '.') {
            return Token(TokenType::NUMBER,parseNumber());
        }
        //情况2：解析运算符
        switch (c) {
            case '+':
                pos++;
                return Token(TokenType::PLUS);
            case '-':{
                size_t minusPos = pos;  // 记录 '-' 的原始位置
                pos++;
                skipWhitespace();
                // 判断是否是负号（而非减号运算符）
                bool isNegativeSign = false;
                // 条件1：'-' 在表达式开头
                if (minusPos == 0) {
                    isNegativeSign = true;
                }
                // 条件2：'-' 前是左括号、运算符、函数字母
                else {
                    char prevChar = expr[minusPos - 1];
                    if (prevChar == '(' || prevChar == '+' || prevChar == '-' ||
                        prevChar == '*' || prevChar == '/' || isalpha(prevChar)) {
                        isNegativeSign = true;
                        }
                }
                // 如果是负号，且后面是数字/小数点 → 解析为负数
                if (isNegativeSign && pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) {
                    double num = parseNumber();
                    return Token(TokenType::NUMBER, -num);
                }
                // 否则是减号运算符
                return Token(TokenType::MINUS);
        }
            case '*':
                pos++;
                return Token(TokenType::MULTIPLY);
            case '/':
                pos++;
                return Token(TokenType::DIVIDE);
            case '(':
                pos++;
                return Token(TokenType::LPAREN);
            case ')':
                pos++;
                return Token(TokenType::RPAREN);
            default:
                if (isalpha(c)) {
                    return Token(parseFunction());
                }
                throw invalid_argument("非法字符"+string(1,c)+"仅支持数字 + - * \ ( ),sin/cos");
        }
    }
};
//***中缀表达试转后缀表达式***

//辅助函数，优先级；
int getPriority(TokenType type) {
    switch (type) {
        case TokenType::SIN:
        case TokenType::COS:
            return 3;
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE:
            return 2;
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 1;
        default:
            return 0;
    }
}

//核心函数 中缀Token转后缀Token
vector<Token> infixToPostfix(Tokenizer& tokenizer) {
    vector<Token> postfix;//存储后缀表达式
    stack<Token> opStack;//存运算符
while (1) {
    Token token = tokenizer.nextToken();//获取下一个Token
    if (token.type == TokenType::END) break;

    switch (token.type) {
        case TokenType::NUMBER:
            postfix.push_back(token);
            break;
        case TokenType::LPAREN:
            opStack.push(token);
            break;
        case TokenType::RPAREN:
                while (!opStack.empty() && opStack.top().type != TokenType::LPAREN) {
                    postfix.push_back(opStack.top());
                    opStack.pop();
                }
            if (opStack.empty()) {
                throw invalid_argument("括号不匹配，缺少左括号");
            }
            opStack.pop();
            break;
        case TokenType::SIN:
        case TokenType::COS:
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE:
        case TokenType::PLUS:
        case TokenType::MINUS:
            while (!opStack.empty() && getPriority(opStack.top().type) >= getPriority(token.type)) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
            break;
        default:
            throw invalid_argument("解析错误：遇到未知Token");
    }
}
while(!opStack.empty()) {
    if (opStack.top().type == TokenType::LPAREN) {
        throw invalid_argument("括号不匹配，缺少右括号");
    }
    postfix.push_back(opStack.top());
    opStack.pop();
}
    return postfix;
}

//后缀表达式求值
double evaluatePostfix(const vector<Token>& postfix) {
    stack<double> numStack;
    for (const Token& token : postfix) {
        switch (token.type) {
            case TokenType::NUMBER:
                numStack.push(token.value);
                break;
            case TokenType::PLUS: {
                if (numStack.size() < 2) {
                    throw invalid_argument("加法错误：缺少操作数");
                }
                double b = numStack.top(); numStack.pop();
                double a = numStack.top(); numStack.pop();
                numStack.push(a + b);
                break;
            }
            case TokenType::MINUS:{
                if (numStack.size() < 2) {
                    throw invalid_argument("减法错误，缺少操作数");
                }
                double b = numStack.top(); numStack.pop();
                double a = numStack.top(); numStack.pop();
                numStack.push(a - b);
                break;
        }
            case TokenType::MULTIPLY: {
                if (numStack.size() < 2) {
                    throw invalid_argument("乘法错误，缺少操作数");
                }
                double b = numStack.top(); numStack.pop();
                double a = numStack.top(); numStack.pop();
                numStack.push(a * b);
                break;
            }
                case TokenType::DIVIDE: {
                if (numStack.size() < 2) {
                    throw invalid_argument("除法错误，缺少操作数");
                }
                double b = numStack.top(); numStack.pop();
                if (fabs(b) < 1e-9) {
                    throw invalid_argument("除法错误，除数不能为0");
                }
                double a = numStack.top(); numStack.pop();
                numStack.push(a / b);
                break;
            }
                case TokenType::SIN: {
                if (numStack.empty()) {
                    throw invalid_argument("sin函数缺少参数");
                }
                double x = numStack.top(); numStack.pop();
                numStack.push(sin(x));
                break;
            }
            case TokenType::COS: {
                if (numStack.empty()) {
                    throw invalid_argument("cos函数缺少参数");
                }
                double x = numStack.top(); numStack.pop();
                numStack.push(cos(x));
                break;
            }
            default:
                throw invalid_argument("求值错误，遇到非法Token");
        }
    }
    if(numStack.size() != 1) {
        throw invalid_argument("表达式格式错误，操作数个数不正确");
    }
    return numStack.top();
}



int main() {
    cout << "=============================================" << endl;
    cout << "          C++命令行计算器（小白友好版）       " << endl;
    cout << "=============================================" << endl;
    cout << "支持功能：" << endl;
    cout << "  1. 四则运算：+ - * /（支持优先级、括号）" << endl;
    cout << "  2. 数学函数：sin/cos（参数为弧度，如sin(3.14159/2)）" << endl;
    cout << "  3. 科学计数法：1e3（1000）、2.5e-4（0.00025）" << endl;
    cout << "  4. 负数：-123、sin(-3.14) " << endl;
    cout << "退出方式：输入 exit/quit 并回车" << endl;
    cout << "=============================================" << endl;
    string input;
while (1) {
    cout << "请输入表达式>";
    getline(cin, input);
    if (input == "exit" || input == "quit" || input == "q") break;
    if (input.empty()) cout << "请输入有效表达式（不能为空）";
    try {
        Tokenizer tokenizer(input);
        vector<Token> postfix = infixToPostfix(tokenizer);
        double result = evaluatePostfix(postfix);
        cout << "计算结果：" << fixed << setprecision(6) << result << endl;
    }catch (const exception& e) {
    cout << "错误：" << e.what() << endl;
    }
}
    return 0;
}
