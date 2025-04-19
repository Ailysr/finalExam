#pragma once
#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>
#include <cmath>
#include <functional>
#include <cctype>
#include <stdexcept>
using namespace std;
class ExpressionParser {
public:
    ExpressionParser(const string& expr) : expression(expr), pos(0), currentX(0.0) {}

    function<double(double)> compile() {
        return [this](double x) {
            this->pos = 0;
            this->currentX = x;
            return parseExpression();
            };
    }

private:
    string expression;
    size_t pos;
    double currentX;

    void skipWhitespace() {
        while (pos < expression.size() && isspace(expression[pos])) pos++;
    }

    double parseNumber() {
        skipWhitespace();
        size_t start = pos;
        bool dotSeen = false;
        while (pos < expression.size() && (isdigit(expression[pos]) || expression[pos] == '.')) {
            if (expression[pos] == '.') {
                if (dotSeen) throw runtime_error("Multiple '.' in number");
                dotSeen = true;
            }
            pos++;
        }
        return stod(expression.substr(start, pos - start));
    }

    string parseIdentifier() {
        skipWhitespace();
        size_t start = pos;
        while (pos < expression.size() && (isalpha(expression[pos]) || expression[pos] == '_'))
            pos++;
        return expression.substr(start, pos - start);
    }

    bool match(char expected) {
        skipWhitespace();
        if (pos < expression.size() && expression[pos] == expected) {
            pos++;
            return true;
        }
        return false;
    }

    double parseExpression() {
        double result = parseTerm();
        while (true) {
            if (match('+')) result += parseTerm();
            else if (match('-')) result -= parseTerm();
            else break;
        }
        return result;
    }

    double parseTerm() {
        double result = parseFactor();
        while (true) {
            if (match('*')) result *= parseFactor();
            else if (match('/')) result /= parseFactor();
            else break;
        }
        return result;
    }

    double parseFactor() {
        double base = parseUnary();
        if (match('^')) {
            double exponent = parseFactor();
            return pow(base, exponent);
        }
        return base;
    }

    double parseUnary() {
        if (match('+')) return parseUnary();
        if (match('-')) return -parseUnary();
        return parsePrimary();
    }

    double parsePrimary() {
        skipWhitespace();

        if (match('(')) {
            double val = parseExpression();
            if (!match(')')) throw runtime_error("Expected ')'");
            return val;
        }

        if (isdigit(expression[pos]) || expression[pos] == '.') {
            return parseNumber();
        }

        if (isalpha(expression[pos])) {
            string id = parseIdentifier();

            if (match('(')) {
                double arg = parseExpression();
                if (!match(')')) throw runtime_error("Expected ')' after function argument");
				// Handle built-in functions, if you want to add more, just add them here
                if (id == "sin") return sin(arg);
                if (id == "cos") return cos(arg);
                if (id == "exp") return exp(arg);
                if (id == "log") return log(arg);
				//帮我添加更多的函数
				if (id == "tan") return tan(arg);
				if (id == "sqrt") return sqrt(arg);
				if (id == "abs") return abs(arg);
				if (id == "asin") return asin(arg);
				if (id == "acos") return acos(arg);
				if (id == "atan") return atan(arg);
				if (id == "sinh") return sinh(arg);
				if (id == "cosh") return cosh(arg);
				if (id == "tanh") return tanh(arg);
				if (id == "floor") return floor(arg);
				if (id == "ceil") return ceil(arg);
				if (id == "round") return round(arg);
				if (id == "exp2") return exp2(arg);
				if (id == "log2") return log2(arg);
                if (id == "log10") return log10(arg);
				
                throw runtime_error("Unknown function: " + id);
            }

            if (id == "x") return currentX;

            throw runtime_error("Unknown identifier: " + id);
        }

        throw runtime_error("Unexpected character at position " + to_string(pos));
    }
};

#endif // EXPRESSION_PARSER_H
