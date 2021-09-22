#include "Emulator/Assembler.h"

using namespace Emulator;

std::map<std::string, std::pair<OpCode, ArgType>> def = {
    {"NOP", {OpCode::NOP, ArgType::NONE}}, 

    {"HALT", {OpCode::HALT, ArgType::NONE}},

    {"SETA", {OpCode::SETA, ArgType::VALUE}},
    {"SETB", {OpCode::SETB, ArgType::VALUE}},
    {"SETC", {OpCode::SETC, ArgType::VALUE}},

    {"LOADA", {OpCode::LOADA, ArgType::POINTER}},
    {"LOADB", {OpCode::LOADB, ArgType::POINTER}},
    {"LOADC", {OpCode::LOADC, ArgType::POINTER}},

    {"STOREA", {OpCode::STOREA, ArgType::POINTER}},
    {"STOREB", {OpCode::STOREB, ArgType::POINTER}},
    {"STOREC", {OpCode::STOREC, ArgType::POINTER}},

    {"PUSHA", {OpCode::PUSHA, ArgType::NONE}},
    {"PUSHB", {OpCode::PUSHB, ArgType::NONE}},
    {"PUSHC", {OpCode::PUSHC, ArgType::NONE}},

    {"POPA", {OpCode::POPA, ArgType::NONE}},
    {"POPB", {OpCode::POPB, ArgType::NONE}},
    {"POPC", {OpCode::POPC, ArgType::NONE}},

    {"MOVCA", {OpCode::MOVCA, ArgType::NONE}},
    {"MOVCB", {OpCode::MOVCB, ArgType::NONE}},
    {"MOVCIDX", {OpCode::MOVCIDX, ArgType::NONE}},

    {"INCA", {OpCode::INCA, ArgType::VALUE}},
    {"INCB", {OpCode::INCB, ArgType::VALUE}},
    {"INCC", {OpCode::INCC, ArgType::VALUE}},

    {"IDXA", {OpCode::IDXA, ArgType::NONE}},
    {"IDXB", {OpCode::IDXB, ArgType::NONE}},
    {"IDXC", {OpCode::IDXC, ArgType::NONE}},

    {"WRITEAX", {OpCode::WRITEAX, ArgType::NONE}},
    {"WRITEBX", {OpCode::WRITEBX, ArgType::NONE}},
    {"WRITECX", {OpCode::WRITECX, ArgType::NONE}},

    {"ADD", {OpCode::ADD, ArgType::NONE}},
    {"SUB", {OpCode::SUB, ArgType::NONE}},
    {"MUL", {OpCode::MUL, ArgType::NONE}},
    {"DIV", {OpCode::DIV, ArgType::NONE}},
    {"IDIV", {OpCode::IDIV, ArgType::NONE}},
    {"MOD", {OpCode::MOD, ArgType::NONE}},
    {"EXP", {OpCode::EXP, ArgType::NONE}},

    {"ATAN", {OpCode::ATAN, ArgType::NONE}},
    {"COS", {OpCode::COS, ArgType::NONE}},
    {"LOG", {OpCode::LOG, ArgType::NONE}},
    {"SIN", {OpCode::SIN, ArgType::NONE}},
    {"SQR", {OpCode::SQR, ArgType::NONE}},
    {"TAN", {OpCode::TAN, ArgType::NONE}},

    {"FLT", {OpCode::FLT, ArgType::NONE}},
    {"INT", {OpCode::INT, ArgType::NONE}},
    {"PTR", {OpCode::PTR, ArgType::NONE}},

    {"AND", {OpCode::AND, ArgType::NONE}},
    {"OR", {OpCode::OR, ArgType::NONE}},
    {"NOT", {OpCode::NOT, ArgType::NONE}},

    {"EQ", {OpCode::EQ, ArgType::NONE}},
    {"NE", {OpCode::NE, ArgType::NONE}},
    {"GT", {OpCode::GT, ArgType::NONE}},
    {"GE", {OpCode::GE, ArgType::NONE}},
    {"LT", {OpCode::LT, ArgType::NONE}},
    {"LE", {OpCode::LE, ArgType::NONE}},
    {"CMP", {OpCode::CMP, ArgType::NONE}},

    {"SETIDX", {OpCode::SETIDX, ArgType::POINTER}},
    {"LOADIDX", {OpCode::LOADIDX, ArgType::POINTER}},
    {"STOREIDX", {OpCode::STOREIDX, ArgType::VALUE}},
    {"INCIDX", {OpCode::INCIDX, ArgType::VALUE}},
    {"SAVEIDX", {OpCode::SAVEIDX, ArgType::POINTER}},
    {"PUSHIDX", {OpCode::PUSHIDX, ArgType::NONE}},
    {"POPIDX", {OpCode::POPIDX, ArgType::NONE}},

    {"JMP", {OpCode::JMP, ArgType::SHORT}},
    {"JMPEZ", {OpCode::JMPEZ, ArgType::SHORT}},
    {"JMPNZ", {OpCode::JMPNZ, ArgType::SHORT}},

    {"IDATA", {OpCode::IDATA, ArgType::SHORT}},
    {"FDATA", {OpCode::FDATA, ArgType::FLOAT}},
    {"PDATA", {OpCode::PDATA, ArgType::POINTER}},
    {"SDATA", {OpCode::SDATA, ArgType::STRING}},

    {"SYSCALL", {OpCode::SYSCALL, ArgType::SYSCALL}},

    {"CALL", {OpCode::CALL, ArgType::SHORT}},
    {"RETURN", {OpCode::RETURN, ArgType::NONE}},

    {"IRQ", {OpCode::IRQ, ArgType::SHORT}},

    {"ALLOC", {OpCode::ALLOC, ArgType::SHORT}},

    {"YIELD", {OpCode::YIELD, ArgType::NONE}},

    {"TRACE", {OpCode::TRACE, ArgType::SHORT}}
};

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isWhitespace(char c) {
    return (c == ' ') || (c == '\t');
}

Emulator::AsmToken parseAsmLine(const std::string &line) {
    size_t i = 0;

    while (i < line.size()) {
        if (isWhitespace(line[i])) {
            i++;
            continue;
        }

        int start = i;

        while (isAlpha(line[i++])) {
        }

        auto opname = line.substr(start, i-start-1);

        std::cerr << opname <<std::endl;

        auto op = def.find(opname);
        if (op == def.end()) {
            std::cerr << "Uknown opcode `" << opname << "'" << std::endl;
            exit(-1);
        }

        auto opcode = op->second.first;
        auto arg = op->second.second;

        if (arg == ArgType::NONE) {
            return Emulator::AsmToken(opcode);
        } else if (arg == ArgType::SHORT) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int numstart = i;

            while (i < line.size()) {
                if (!isDigit(line[i])) {
                    std::cerr << "Expected arg: INT  for " << opname << std::endl;
                    exit(-1);
                }
                i++;
            }

            std::cerr << line.substr(numstart, i-numstart) << std::endl;

            return Emulator::AsmToken(opcode, (int16_t)std::stoi(line.substr(numstart, i-numstart)));
        } else if (arg == ArgType::FLOAT) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int numstart = i;

            while (isDigit(line[i]))
                i++;

            if (line[i] == '.' && isDigit(line[i+1])) {
                i++;

                while (isDigit(line[i]))
                    i++;
            } else {
                std::cerr << "Expected arg: FLOAT  for " << opname << std::endl;
                exit(-1);
            }

            std::cerr << line.substr(numstart, i-numstart) << std::endl;

            return Emulator::AsmToken(opcode, (float)std::stof(line.substr(numstart, i-numstart)));

        } else {
            std::cerr << "TODO " << opname;
            exit(-1);
        }
    }
}

std::vector<Emulator::AsmToken> parseAsmFile(const std::string &filename) {
    std::vector<Emulator::AsmToken> tokens;
    std::string line;
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "Could not open `" << filename << "'" << std::endl;
        exit(-1);
    }

    while (std::getline(infile, line)) {
        auto token = parseAsmLine(line);
        tokens.push_back(token);
    }

    return tokens;
}

void assemble(std::vector<Emulator::AsmToken> lines, Emulator::Program &program) {
    for (auto token : lines) {
        if (token.arg == std::nullopt) {
            program.add(token.opcode);
        } else {
            if (std::holds_alternative<int16_t>(*token.arg)) {
                auto value = std::get<int16_t>(*token.arg);
                program.addShort(token.opcode, value);
            } else if (std::holds_alternative<float>(*token.arg)) {
                auto value = std::get<float>(*token.arg);
                program.addFloat(token.opcode, value);
            } else {
            }
        } 
    }
}

