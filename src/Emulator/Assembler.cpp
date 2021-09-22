#include "Emulator/Assembler.h"

using namespace Emulator;

static std::map<std::string, uint32_t> labels;
static std::map<uint32_t, std::string> jumps;

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

    {"JMP", {OpCode::JMP, ArgType::LABEL}},
    {"JMPEZ", {OpCode::JMPEZ, ArgType::LABEL}},
    {"JMPNZ", {OpCode::JMPNZ, ArgType::LABEL}},

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

std::pair<SysCall, RuntimeValue> getSyscall(const std::string &syscallname, const std::string &rtname) {
    RuntimeValue rt;
    SysCall syscall;

    if (syscallname == "CLS") {
        syscall = SysCall::CLS;
    } else if (syscallname == "WRITE") {
        syscall = SysCall::WRITE;
    } else if (syscallname == "READ") {
        syscall = SysCall::READ;
    } else if (syscallname == "READKEY") {
        syscall = SysCall::READKEY;
    } else if (syscallname == "DRAW") {
        syscall = SysCall::DRAW;
    } else if (syscallname == "DRAWLINE") {
        syscall = SysCall::DRAWLINE;
    } else if (syscallname == "BLIT") {
        syscall = SysCall::BLIT;
    } else {
        std::cerr << "Unknown SysCall " << syscallname << std::endl;
        exit(-1);
    }

    if (rtname == "NONE") {
        rt = RuntimeValue::NONE;
    } else if (rtname == "A") {
        rt = RuntimeValue::A;
    } else if (rtname == "B") {
        rt = RuntimeValue::B;
    } else if (rtname == "C") {
        rt = RuntimeValue::C;
    } else if (rtname == "IDX") {
        rt = RuntimeValue::IDX;
    } else {
        std::cerr << "Unknown RuntimeValue " << syscallname << std::endl;
        exit(-1);
    }

    return std::pair<SysCall, RuntimeValue>(syscall, rt);
}

static Emulator::AsmToken parseAsmLine(const std::string &line, uint32_t offset, uint32_t linenum, uint32_t statement) {
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

            if (!(line[i] == '-' || isDigit(line[i])))
                std::cerr << "Expected arg: INT  for " << opname << std::endl;
            i++;

            while (i < line.size()) {
                if (!isDigit(line[i])) {
                    std::cerr << "Expected arg: INT  for " << opname << std::endl;
                    exit(-1);
                }
                i++;
            }

            return Emulator::AsmToken(opcode, (int16_t)std::stoi(line.substr(numstart, i-numstart)));
        } else if (arg == ArgType::POINTER) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int numstart = i;

            while (i < line.size()) {
                if (!isDigit(line[i])) {
                    std::cerr << "Expected arg: UINT  for " << opname << std::endl;
                    exit(-1);
                }
                i++;
            }

            return Emulator::AsmToken(opcode, (int32_t)std::stoi(line.substr(numstart, i-numstart)));
        } else if (arg == ArgType::FLOAT) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int numstart = i;

            if (!(line[i] == '-' || isDigit(line[i])))
                std::cerr << "Expected arg: VALUE for " << opname << std::endl;
            i++;

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

            return Emulator::AsmToken(opcode, (float)std::stof(line.substr(numstart, i-numstart)));
        } else if (arg == ArgType::VALUE) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int numstart = i;

            if (!(line[i] == '-' || isDigit(line[i])))
                std::cerr << "Expected arg: FLOAT for " << opname << std::endl;
            i++;

            while (isDigit(line[i]))
                i++;

            if (line[i] == '.' && isDigit(line[i+1])) {
                i++;

                while (isDigit(line[i]))
                    i++;
                return Emulator::AsmToken(opcode, FloatAsValue((float)std::stof(line.substr(numstart, i-numstart))));
            } else {
                return Emulator::AsmToken(opcode, IntAsValue(std::stoi(line.substr(numstart, i-numstart))));
            }

        } else if (arg == ArgType::SYSCALL) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int syscallstart = i;

            while (isAlpha(line[i++])) {
            }

            auto syscall = line.substr(syscallstart, i-syscallstart-1);

            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int rtstart = i;

            while (isAlpha(line[i++])) {
            }

            auto rt  = line.substr(rtstart, i-rtstart-1);

            return Emulator::AsmToken(opcode, getSyscall(syscall, rt));
        } else if (arg == ArgType::STRING) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            if (line[i] != '"') {
                std::cerr << "Expected arg: STRING  for " << opname << std::endl;
                exit(-1);
            }

            i++;

            std::string str;
            while (line[i++] != '"') {
                char c = line[i-1];

                if (c == '\\' && line[i] == 'n') {
                    c = '\n';
                    i++;
                } else if (c == '\\' && line[i] == '"') {
                    c = '"';
                    i++;
                }

                str += c;
            }

            return Emulator::AsmToken(opcode, str);
        } else if (arg == ArgType::LABEL) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            auto labelstart = i;
            while (isAlpha(line[i++])) {
            }

            auto label = line.substr(labelstart, i-labelstart-1);
            auto jump = labels.find(label);

            if (jump != labels.end()) {
                return Emulator::AsmToken(opcode, (int16_t)jump->second);
            }

            jumps[statement] = label;

            return Emulator::AsmToken(opcode, (int16_t)0);
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

    uint32_t offset = 0;
    uint32_t linenum = 0;
    uint32_t statement = 0;

    if (!infile.is_open()) {
        std::cerr << "Could not open `" << filename << "'" << std::endl;
        exit(-1);
    }

    labels.clear();

    while (std::getline(infile, line)) {
        linenum++;

        auto pos = line.find_last_not_of("\n \t");
        if (pos != std::string::npos)
            line = line.substr(0, pos+1);

        if (line[0] == ';')
            continue;

        if (line[line.size()-1] == ':') {
            auto label = line.substr(0, line.size()-1);
            labels[label] = offset;
            continue;
        }

        auto token = parseAsmLine(line, offset, linenum, statement);
        statement++;
        offset += token.size();
        tokens.push_back(token);
    }

    for (auto jump : jumps) {
        auto label = labels.find(jump.second);

        if (label == labels.end()) {
            std::cerr << "Unknown label `" << jump.second << "'" << std::endl;
            exit(-1);
        }

        tokens[jump.first].arg = (int16_t)label->second;
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
            } else if (std::holds_alternative<int32_t>(*token.arg)) {
                auto value = std::get<int32_t>(*token.arg);
                program.addPointer(token.opcode, (vmpointer_t)value);
            } else if (std::holds_alternative<value_t>(*token.arg)) {
                auto value = std::get<value_t>(*token.arg);
                program.addValue(token.opcode, value);
            } else if (std::holds_alternative<std::string>(*token.arg)) {
                auto value = std::get<std::string>(*token.arg);
                program.addString(token.opcode, value);
            } else if (std::holds_alternative<std::pair<SysCall, RuntimeValue>>(*token.arg)) {
                auto value = std::get<std::pair<SysCall, RuntimeValue>>(*token.arg);
                program.addSyscall(token.opcode, value.first, value.second);
            }
        } 
    }
}

