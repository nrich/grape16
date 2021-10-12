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
        USRFUNCTION,

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
        QUOTE,

        BEEP,
        CLS,
        COLOR,
        DATA,
        DEF,
        DIM,
        ELSE,
        END,
        FOR,
        GOSUB,
        GOTO,
        IF,
        INPUT,
        LET,
        LINE,
        NEXT,
        PALETTE,
        POKE,
        PRINT,
        PSET,
        PUT,
        RANDOMIZE,
        READ,
        REM,
        RETURN,
        SOUND,
        STEP,
        SWAP,
        THEN,
        TIMER,
        TO,
        WAIT,
        WHILE,
        WEND,

        VOICE,

        EOL,

        TOKEN_COUNT
    };

    struct BasicToken {
        BasicTokenType type;
        std::string str;
        int lbp;
        BasicToken(BasicTokenType type, const std::string &str, const int lbp=Precedence::NONE) : type(type), str(str), lbp(lbp) {
        }

        std::string toString() const {
            if (type == BasicTokenType::STRING) {
                if (str.empty())
                    return "\"\"";

                std::string res = "\"";

                for (const auto &c : str) {
                    switch (c) {
                        case '\n':
                            res += '\\';
                            res += 'n';
                            break;
                        case '\t':
                            res += '\\';
                            res += 't';
                            break;
                        case '\\':
                            res += '\\';
                            res += '\\';
                            break;
                        case '"':
                            res += '\\';
                            res += '"';
                            break;
                        default:
                            res += c;
                    }
                } 

                res += "\"";

                return res;
            }
            return str;
        }
    };
};

std::pair<uint32_t, std::vector<Emulator::BasicToken>> parseLine(const std::string &line);
std::map<uint32_t, std::vector<Emulator::BasicToken>> parseFile(const std::string &filename);

void compile(const std::map<uint32_t, std::vector<Emulator::BasicToken>> &lines, Emulator::Program &program);

#endif //__EMULATOR_BASIC_H__
