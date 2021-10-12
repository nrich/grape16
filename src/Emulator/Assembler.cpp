#include "Emulator/Assembler.h"

#include <sstream>
#include <iomanip>

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

    {"RND", {OpCode::RND, ArgType::NONE}},
    {"SEED", {OpCode::SEED, ArgType::NONE}},

    {"FLT", {OpCode::FLT, ArgType::NONE}},
    {"INT", {OpCode::INT, ArgType::NONE}},
    {"PTR", {OpCode::PTR, ArgType::NONE}},
    {"STR", {OpCode::STR, ArgType::NONE}},
    {"VSTR", {OpCode::VSTR, ArgType::NONE}},

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

    {"CALL", {OpCode::CALL, ArgType::LABEL}},
    {"RETURN", {OpCode::RETURN, ArgType::NONE}},

    {"IRQ", {OpCode::IRQ, ArgType::SHORT}},

    {"ALLOC", {OpCode::ALLOC, ArgType::SHORT}},
    {"CALLOC", {OpCode::CALLOC, ArgType::NONE}},

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

static std::string str_toupper(std::string s) {
    std::transform(
        s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::toupper(c); }
    );

    return s;
}

static void error(uint32_t linenumber, const std::string &opcode, const std::string &err) {
    std::ostringstream s;
    s << "[" << linenumber << "] " << opcode << ": " << err;
    throw std::domain_error(s.str());
}

std::pair<SysCall, RuntimeValue> getSysCall(uint32_t linenumber, const std::string &opcode, const std::string &syscallname, const std::string &rtname) {
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
    } else if (syscallname == "COLOUR") {
        syscall = SysCall::COLOUR;
    } else if (syscallname == "PALETTE") {
        syscall = SysCall::PALETTE;
    } else if (syscallname == "DRAW") {
        syscall = SysCall::DRAW;
    } else if (syscallname == "DRAWLINE") {
        syscall = SysCall::DRAWLINE;
    } else if (syscallname == "DRAWBOX") {
        syscall = SysCall::DRAWBOX;
    } else if (syscallname == "BLIT") {
        syscall = SysCall::BLIT;
    } else if (syscallname == "SOUND") {
        syscall = SysCall::SOUND;
    } else if (syscallname == "VOICE") {
        syscall = SysCall::VOICE;
    } else {
        error(linenumber, opcode, "Unknown SysCall");
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
        error(linenumber, opcode, "Unknown RuntimeValue");
    }

    return std::pair<SysCall, RuntimeValue>(syscall, rt);
}

std::pair<std::string, std::string> getSysCall(SysCall syscall, RuntimeValue rt) {
    std::string syscallname;
    std::string rtname;

    if (syscall == SysCall::CLS) {
        syscallname = "CLS";
    } else if (syscall == SysCall::WRITE) {
        syscallname = "WRITE";
    } else if (syscall == SysCall::READ) {
        syscallname = "READ";
    } else if (syscall == SysCall::READKEY) {
        syscallname = "READKEY";
    } else if (syscall == SysCall::PALETTE) {
        syscallname = "COLOUR";
    } else if (syscall == SysCall::COLOUR) {
        syscallname = "PALETTE";
    } else if (syscall == SysCall::DRAW) {
        syscallname = "DRAW";
    } else if (syscall == SysCall::DRAWLINE) {
        syscallname = "DRAWLINE";
    } else if (syscall == SysCall::DRAWBOX) {
        syscallname = "DRAWBOX";
    } else if (syscall == SysCall::BLIT) {
        syscallname = "BLIT";
    } else if (syscall == SysCall::SOUND) {
        syscallname = "SOUND";
    } else if (syscall == SysCall::VOICE) {
        syscallname = "VOICE";
    } else {
        throw std::domain_error("Unknown SysCall");
    }

    if (rt == RuntimeValue::NONE) {
        rtname = "NONE";
    } else if (rt == RuntimeValue::A) {
        rtname = "A";
    } else if (rt == RuntimeValue::B) {
        rtname = "B";
    } else if (rt == RuntimeValue::C) {
        rtname = "C";
    } else if (rt == RuntimeValue::IDX) {
        rtname = "IDX";
    } else {
        throw std::domain_error("Unknown RuntimeValue");
    }

    return std::pair<std::string, std::string>(syscallname, rtname);
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

        auto opname = str_toupper(line.substr(start, i-start-1));

        auto op = def.find(opname);
        if (op == def.end()) {
            error(linenum, opname, "Uknown opcode");
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
                error(linenum, opname, "INT expected");
            i++;

            while (i < line.size()) {
                if (!isDigit(line[i])) {
                    error(linenum, opname, "INT expected");
                }
                i++;
            }

            return Emulator::AsmToken(opcode, (int16_t)std::stoi(line.substr(numstart, i-numstart)));
        } else if (arg == ArgType::POINTER) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            if (line.size() > 2 && line[i] == '0' && line[i+1] =='x') {
                i+=2;
                int numstart = i;

                while (isDigit(line[i]))
                    i++;

                return Emulator::AsmToken(opcode, (int32_t)(std::strtoul(line.substr(numstart).c_str(), nullptr, 16)));
            } else {
                error(linenum, opname, "POINTER expected");
            }
        } else if (arg == ArgType::FLOAT) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int numstart = i;

            if (!(line[i] == '-' || isDigit(line[i])))
                error(linenum, opname, "FLOAT expected");
            i++;

            while (isDigit(line[i]))
                i++;

            if (line[i] == '.' && isDigit(line[i+1])) {
                i++;

                while (isDigit(line[i]))
                    i++;
            } else {
                error(linenum, opname, "POINTER expected");
                exit(-1);
            }

            return Emulator::AsmToken(opcode, (float)std::stof(line.substr(numstart)));
        } else if (arg == ArgType::VALUE) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            int numstart = i;

            if (line.size() > 2 && line[i] == '0' && line[i+1] =='x') {
                i+=2;
                numstart = i;

                while (isDigit(line[i]))
                    i++;

                return Emulator::AsmToken(opcode, PointerAsValue(std::strtoul(line.substr(numstart).c_str(), nullptr, 16)));
            }

            if (!(line[i] == '-' || isDigit(line[i])))
                error(linenum, opname, "FLOAT expected");
            i++;

            while (isDigit(line[i]))
                i++;

            if (line[i] == '.' && isDigit(line[i+1])) {
                i++;

                while (isDigit(line[i]))
                    i++;
                return Emulator::AsmToken(opcode, FloatAsValue((float)std::stof(line.substr(numstart))));
            } else {
                return Emulator::AsmToken(opcode, IntAsValue((int16_t)std::stoi(line.substr(numstart))));
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

            return Emulator::AsmToken(opcode, getSysCall(linenum, opname, syscall, rt));
        } else if (arg == ArgType::STRING) {
            while (isWhitespace(line[i])) {
                i++;
                continue;
            }

            if (line[i] != '"') {
                error(linenum, opname, "STRING expected");
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
                } else if (c == '\\' && line[i] == 't') {
                    c = '\t';
                    i++;
                } else if (c == '\\' && line[i] == '\\') {
                    c = '\\';
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

            if (isDigit(line[i])) {
                auto numstart = i;

                while (isDigit(line[i++])) {
                }

                auto jmp = (int16_t)std::stoi(line.substr(numstart));

                return Emulator::AsmToken(opcode, jmp);
            }

            auto labelstart = i;
            while (isAlpha(line[i++]) || isDigit(line[i++])) {
            }

            auto label = line.substr(labelstart, i-labelstart-1);
            auto jump = labels.find(label);

            if (jump != labels.end()) {
                return Emulator::AsmToken(opcode, (int16_t)jump->second);
            }

            jumps[statement] = label;

            return Emulator::AsmToken(opcode, (int16_t)0);
        } else {
            error(linenum, opname, "TODO");
            return Emulator::AsmToken(OpCode::NOP);
        }
    }

    throw std::domain_error("This should never be reached");
    return Emulator::AsmToken(OpCode::NOP);
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
            } else {
                std::cerr << "Error in assemble" << std::endl;
            }
        } 
    }
}

std::vector<Emulator::AsmToken> disassemble(const Emulator::Program &program) {
    std::vector<Emulator::AsmToken> tokens;

    size_t pc = 0;
    while (pc < program.size()) {
        OpCode opcode = program.fetch(pc++);
        auto opname = OpCodeAsString(opcode);
        auto op = def[opname];
        auto arg = op.second;

        if (arg == ArgType::NONE) {
            tokens.push_back(Emulator::AsmToken(opcode));
        } else if (arg == ArgType::SHORT) {
            tokens.push_back(Emulator::AsmToken(opcode, program.readShort(pc)));
            pc += 2;
        } else if (arg == ArgType::FLOAT) {
            tokens.push_back(Emulator::AsmToken(opcode, program.readFloat(pc)));
            pc += 4;
        } else if (arg == ArgType::POINTER) {
            tokens.push_back(Emulator::AsmToken(opcode, (int32_t)program.readPointer(pc)));
            pc += 3;
        } else if (arg == ArgType::VALUE) {
            tokens.push_back(Emulator::AsmToken(opcode, program.readValue(pc)));
            pc += 4;
        } else if (arg == ArgType::SYSCALL) {
            tokens.push_back(Emulator::AsmToken(opcode, std::pair<SysCall, RuntimeValue>((SysCall)program.readShort(pc), (RuntimeValue)program.readShort(pc+2))));
            pc += 2+2;
        } else if (arg == ArgType::STRING) {
            std::string str = "";
            while (uint8_t b = program.readByte(pc)) {
                str += (char)b;
                pc += 1;
            }

            tokens.push_back(Emulator::AsmToken(opcode, str));
        } else if (arg == ArgType::LABEL) {
            tokens.push_back(Emulator::AsmToken(opcode, program.readShort(pc)));
            pc += 2;
        } else {
            std::cerr << "Error in disassemble" << std::endl;
        }
    }

    return tokens;
}

std::string AsmTokenAsString(const Emulator::AsmToken &token) {
    std::ostringstream s;

    s << Emulator::OpCodeAsString(token.opcode);

    if (token.arg != std::nullopt) {
        s << " ";

        if (std::holds_alternative<int16_t>(*token.arg)) {
            auto value = std::get<int16_t>(*token.arg);
            s << value;
        } else if (std::holds_alternative<float>(*token.arg)) {
            auto value = std::get<float>(*token.arg);
            s << value;
        } else if (std::holds_alternative<int32_t>(*token.arg)) {
            auto value = std::get<int32_t>(*token.arg);
            s << "0x" << std::setfill('0') << std::setw(6) << std::uppercase << std::hex << (vmpointer_t)value;
        } else if (std::holds_alternative<value_t>(*token.arg)) {
            auto value = std::get<value_t>(*token.arg);

            if (IS_POINTER(value))
                s << "0x" << std::setfill('0') << std::setw(6) << std::uppercase << std::hex << ValueAsPointer(value);
            else
                s << Emulator::ValueToString(value);
        } else if (std::holds_alternative<std::string>(*token.arg)) {
            auto value = std::get<std::string>(*token.arg);
            std::string res;

            for (const auto &c : value) {
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

            s << "\"" << res << "\"";
        } else if (std::holds_alternative<std::pair<SysCall, RuntimeValue>>(*token.arg)) {
            auto value = std::get<std::pair<SysCall, RuntimeValue>>(*token.arg);
            auto syscall = getSysCall(value.first, value.second);
            s << syscall.first << " " << syscall.second;
        }
    }

    return s.str();
}

Emulator::Program optimize(const Emulator::Program &code) {
    Emulator::Program program(code);

    size_t pc = 0;
    while (pc < program.size()) {
        OpCode current = program.fetch(pc++);
        auto opname = OpCodeAsString(current);
        auto op = def[opname];
        auto arg = op.second;

        if (arg == ArgType::NONE) {
            OpCode next = program.fetch(pc);

            if (current == OpCode::PUSHC && next == OpCode::POPC) {
                program.update(pc-1, OpCode::NOP);
                program.update(pc, OpCode::NOP);
            } else if (current == OpCode::PUSHC && next == OpCode::POPA) {
                program.update(pc-1, OpCode::MOVCA);
                program.update(pc, OpCode::NOP);
            } else if (current == OpCode::PUSHC && next == OpCode::POPB) {
                program.update(pc-1, OpCode::MOVCB);
                program.update(pc, OpCode::NOP);
            } else if (current == OpCode::PUSHC && next == OpCode::POPIDX) {
                program.update(pc-1, OpCode::MOVCIDX);
                program.update(pc, OpCode::NOP);
            }
        } else if (arg == ArgType::SHORT) {
            OpCode next = program.fetch(pc+2);

            if (current == OpCode::SETC && next == OpCode::POPC) {
                program.update(pc+2, OpCode::NOP);
            } else if (current == OpCode::SETC && next == OpCode::POPA) {
                program.update(pc-1, OpCode::SETA);
                program.update(pc+2, OpCode::NOP);
            } else if (current == OpCode::SETC && next == OpCode::POPB) {
                program.update(pc-1, OpCode::SETB);
                program.update(pc+2, OpCode::NOP);
            } else if (current == OpCode::SETC && next == OpCode::POPIDX) {
                program.update(pc-1, OpCode::SETIDX);
                program.update(pc+2, OpCode::NOP);
            }

            pc += 2;
        } else if (arg == ArgType::FLOAT) {
            pc += 4;
        } else if (arg == ArgType::POINTER) {
            pc += 3;
        } else if (arg == ArgType::VALUE) {
            pc += 4;
        } else if (arg == ArgType::SYSCALL) {
            pc += 2+2;
        } else if (arg == ArgType::STRING) {
            while (program.readByte(pc)) {
                pc += 1;
            }
        } else if (arg == ArgType::LABEL) {
            pc += 2;
        } else {
            std::cerr << "Error in disassemble" << std::endl;
        }
    }

    return program;
}

std::string ProgramAsString(const Emulator::Program &code, bool resolve_jumps) {
    std::ostringstream s;
    Emulator::Program program(code);

    std::map<uint32_t, std::string> labels;

    if (resolve_jumps) {
        size_t pc = 0;
        while (pc < program.size()) {
            OpCode opcode = program.fetch(pc++);
            auto opname = OpCodeAsString(opcode);
            auto op = def[opname];
            auto arg = op.second;

            if (arg == ArgType::NONE) {
            } else if (arg == ArgType::SHORT) {
                pc += 2;
            } else if (arg == ArgType::FLOAT) {
                pc += 4;
            } else if (arg == ArgType::POINTER) {
                pc += 3;
            } else if (arg == ArgType::VALUE) {
                pc += 4;
            } else if (arg == ArgType::SYSCALL) {
                pc += 2+2;
            } else if (arg == ArgType::STRING) {
                while (program.readByte(pc)) {
                    pc += 1;
                }
            } else if (arg == ArgType::LABEL) {
                auto dst = program.readShort(pc);
                //labels[dst] = std::string("LABEL_") + std::to_string(dst);
                labels[dst] = std::string("LABEL_") + std::to_string(labels.size());
                pc += 2;
            } else {
                std::cerr << "Error in disassemble" << std::endl;
            }
        }
    }

    size_t pc = 0;
    while (pc < program.size()) {
        OpCode opcode = program.fetch(pc++);
        auto opname = OpCodeAsString(opcode);
        auto op = def[opname];
        auto arg = op.second;

        if (resolve_jumps) {
            auto label = labels.find(pc-1);

            if (label != labels.end()) {
                s << label->second << ":" << std::endl;
            }
        }

        if (arg == ArgType::NONE) {
            s << AsmTokenAsString(Emulator::AsmToken(opcode)) << std::endl;
        } else if (arg == ArgType::SHORT) {
            s << AsmTokenAsString(Emulator::AsmToken(opcode, program.readShort(pc))) << std::endl;
            pc += 2;
        } else if (arg == ArgType::FLOAT) {
            s << AsmTokenAsString(Emulator::AsmToken(opcode, program.readFloat(pc))) << std::endl;
            pc += 4;
        } else if (arg == ArgType::POINTER) {
            s << AsmTokenAsString(Emulator::AsmToken(opcode, (int32_t)program.readPointer(pc))) << std::endl;
            pc += 3;
        } else if (arg == ArgType::VALUE) {
            s << AsmTokenAsString(Emulator::AsmToken(opcode, program.readValue(pc))) << std::endl;
            pc += 4;
        } else if (arg == ArgType::SYSCALL) {
            s << AsmTokenAsString(Emulator::AsmToken(opcode, std::pair<SysCall, RuntimeValue>((SysCall)program.readShort(pc), (RuntimeValue)program.readShort(pc+2)))) << std::endl;
            pc += 2+2;
        } else if (arg == ArgType::STRING) {
            std::string str = "";
            while (uint8_t b = program.readByte(pc)) {
                str += (char)b;
                pc += 1;
            }

            s << AsmTokenAsString(Emulator::AsmToken(opcode, str)) << std::endl;
        } else if (arg == ArgType::LABEL) {
            if (resolve_jumps) {
                auto label = labels.find(program.readShort(pc));

                if (label != labels.end()) {
                    s << AsmTokenAsString(Emulator::AsmToken(opcode)) << " " << label->second << std::endl;
                } else {
                    s << AsmTokenAsString(Emulator::AsmToken(opcode, program.readShort(pc))) << std::endl;
                }

            } else {
                s << AsmTokenAsString(Emulator::AsmToken(opcode, program.readShort(pc))) << std::endl;
            }
            pc += 2;
        } else {
            std::cerr << "Error in disassemble" << std::endl;
        }
    }

    return s.str();
}
