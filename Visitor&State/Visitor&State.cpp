#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <termcolor/termcolor.hpp>

class Token;
class NumberToken;
class BraceToken;
class OperationToken;

class TokenVisitor {
public:
    virtual void visit(NumberToken& token) = 0;
    virtual void visit(BraceToken& token) = 0;
    virtual void visit(OperationToken& token) = 0;
};

class Token {
public:
    virtual void accept(TokenVisitor& visitor) = 0;
};

class NumberToken : public Token {
public:
    int value;

    NumberToken(int val) : value(val) {}

    void accept(TokenVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class BraceToken : public Token {
public:
    char brace;

    BraceToken(char b) : brace(b) {}

    void accept(TokenVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class OperationToken : public Token {
public:
    char operation;

    OperationToken(char op) : operation(op) {}

    void accept(TokenVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class ParserVisitor : public TokenVisitor {
private:
    std::vector<Token*> output;
    std::stack<Token*> operators;

    int priority(char op) {
        if (op == '*' || op == '/') {
            return 2;
        }
        else if (op == '+' || op == '-') {
            return 1;
        }
        else {
            return 0;
        }
    }

public:
    void visit(NumberToken& token) override {
        output.push_back(&token);
    }

    void visit(BraceToken& token) override {
        if (token.brace == '(') {
            operators.push(&token);
        }
        else {
            while (!operators.empty() && dynamic_cast<BraceToken*>(operators.top()) == nullptr) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
        }
    }

    void visit(OperationToken& token) override {
        while (!operators.empty() && dynamic_cast<OperationToken*>(operators.top()) != nullptr
            && priority(token.operation) <= priority(dynamic_cast<OperationToken*>(operators.top())->operation)) {
            output.push_back(operators.top());
            operators.pop();
        }
        operators.push(&token);
    }

    std::vector<Token*> getOutput() {
        while (!operators.empty()) {
            output.push_back(operators.top());
            operators.pop();
        }
        return output;
    }
};

class PrintVisitor : public TokenVisitor {
public:
    void visit(NumberToken& token) override {
        std::cout << termcolor::green << token.value << " " << termcolor::reset;
    }

    void visit(BraceToken& token) override {
        std::cout << termcolor::cyan << token.brace << " " << termcolor::reset;
    }

    void visit(OperationToken& token) override {
        std::cout << termcolor::yellow << token.operation << " " << termcolor::reset;
    }
};

class CalcVisitor : public TokenVisitor {
private:
    std::stack<int> stack;

public:
    void visit(NumberToken& token) override {
        stack.push(token.value);
    }

    void visit(BraceToken& token) override {}

    void visit(OperationToken& token) override {
        int b = stack.top();
        stack.pop();
        int a = stack.top();
        stack.pop();

        if (token.operation == '/' && b == 0) {
            std::cout << termcolor::red << "Ошибка: деление на ноль" << termcolor::reset << std::endl;
            exit(1);
        }

        switch (token.operation) {
        case '+':
            stack.push(a + b);
            break;
        case '-':
            stack.push(a - b);
            break;
        case '*':
            stack.push(a * b);
            break;
        case '/':
            stack.push(a / b);
            break;
        }
    }

    int getResult() {
        return stack.top();
    }
};

class Tokenizer {
public:
    std::vector<Token*> tokenize(std::string input) {
        std::istringstream iss(input);
        char c;
        std::vector<Token*> tokens;
        while (iss.get(c)) {
            if (isdigit(c)) {
                iss.putback(c);
                int num;
                iss >> num;
                tokens.push_back(new NumberToken(num));
            }
            else if (c == '(' || c == ')') {
                tokens.push_back(new BraceToken(c));
            }
            else if (c == '+' || c == '-' || c == '*' || c == '/') {
                tokens.push_back(new OperationToken(c));
            }
            else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                std::cout << termcolor::red << "Ошибка: Введен недопустимый символ '" << c << "', пожалуйста, введите корректное выражение." << termcolor::reset << std::endl;
                tokens.clear();
                break;
            }
        }
        return tokens;
    }

};

int main() {
    setlocale(LC_ALL, "ru");
    std::string input;

    std::cout << termcolor::bold << termcolor::yellow << "Visitor&State Калькулятор" << termcolor::reset << std::endl;

    while (true) {
        std::cout << termcolor::cyan << "Введите выражение (или выйдите из программы 'exit'): " << termcolor::reset;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }
        else if (input == "") {  
            std::cout << termcolor::red << "Не вернные значения, попробуйте ещё раз." << termcolor::reset << std::endl;
            continue;
        }
        else {
            Tokenizer tokenizer;
            ParserVisitor parser;
            PrintVisitor printer;

            std::vector<Token*> tokens = tokenizer.tokenize(input);
            for (auto token : tokens) {
                token->accept(parser);
            }

            tokens = parser.getOutput();
            for (auto token : tokens) {
                token->accept(printer);
            }
            std::cout << std::endl;

            CalcVisitor calc;
            for (auto token : tokens) {
                token->accept(calc);
            }

            std::cout << termcolor::bold << termcolor::green << "Result: " << calc.getResult() << termcolor::reset << std::endl;
        }
    }

    return 0;
}
