#ifndef __EMULATOR_BASIC_H__
#define __EMULATOR_BASIC_H__

#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <iostream>

#include "Emulator/VM.h"

namespace Emulator {
    enum Precedence {
        NONE = 0,
        ASSIGNMENT,
        OR,
        AND,
        EQUALITY,
        COMPARISON,
        TERM,
        FACTOR,
        UNARY,
        CALL,
        PRIMARY
    };

    enum class BasicTokenType {
        IDENTIFIER,

        STRING,
        FLOAT,
        INT,
        FUNCTION,

        EQUAL,
        GREATER,
        LESS,
        NOT_EQUAL,
        GREATER_EQUAL,
        LESS_EQUAL,

        AND,
        OR,
        NOT,

        PLUS,
        MINUS,
        STAR,
        SLASH,
        CARAT,
        BACKSLASH,
        PERCENT,

        LEFT_PAREN,
        RIGHT_PAREN,

        SEMICOLON,
        COLON,
        COMMA,

        CLS,
        DATA,
        DIM,
        ELSE,
        END,
        FOR,
        GOSUB,
        GOTO,
        IF,
        INPUT,
        LET,
        NEXT,
        POKE,
        PRINT,
        PSET,
        READ,
        REM,
        RETURN,
        STEP,
        SWAP,
        THEN,
        TO,
        WHILE,
        WEND,

        LINE,

        EOL,

        TOKEN_COUNT
    };

    struct BasicToken {
        BasicTokenType type;
        std::string str;
        int lbp;
        BasicToken(BasicTokenType type, const std::string &str, const int lbp=Precedence::NONE) : type(type), str(str), lbp(lbp) {
        }
    };
};

std::pair<uint32_t, std::vector<Emulator::BasicToken>> parseLine(const std::string &line);
std::map<uint32_t, std::vector<Emulator::BasicToken>> parseFile(const std::string &filename);

void compile(const std::map<uint32_t, std::vector<Emulator::BasicToken>> &lines, Emulator::Program &program);

#endif //__EMULATOR_BASIC_H__
