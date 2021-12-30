#include "Emulator/VM.h"

#include <ctime>
#include <cmath>
#include <algorithm>

using namespace Emulator;

#ifdef SYS32
    #define _INT_MAX INT32_MAX
    #define _INT_MIN INT32_MIN
#else
    #define _INT_MAX INT16_MAX
    #define _INT_MIN INT16_MIN
#endif


std::string Emulator::OpCodeAsString(OpCode opcode) {
    switch(opcode) {
        case OpCode::NOP: return "NOP";
        case OpCode::HALT: return "HALT";
        case OpCode::SETA: return "SETA";
        case OpCode::SETB: return "SETB";
        case OpCode::SETC: return "SETC";
        case OpCode::LOADA: return "LOADA";
        case OpCode::LOADB: return "LOADB";
        case OpCode::LOADC: return "LOADC";
        case OpCode::STOREA: return "STOREA";
        case OpCode::STOREB: return "STOREB";
        case OpCode::STOREC: return "STOREC";
        case OpCode::READA: return "READA";
        case OpCode::READB: return "READB";
        case OpCode::READC: return "READC";
        case OpCode::WRITEA: return "WRITEA";
        case OpCode::WRITEB: return "WRITEB";
        case OpCode::WRITEC: return "WRITEC";
        case OpCode::PUSHA: return "PUSHA";
        case OpCode::PUSHB: return "PUSHB";
        case OpCode::PUSHC: return "PUSHC";
        case OpCode::POPA: return "POPA";
        case OpCode::POPB: return "POPB";
        case OpCode::POPC: return "POPC";
        case OpCode::MOVCA: return "MOVCA";
        case OpCode::MOVCB: return "MOVCB";
        case OpCode::MOVCIDX: return "MOVCIDX";
        case OpCode::INCA: return "INCA";
        case OpCode::INCB: return "INCB";
        case OpCode::INCC: return "INCC";
        case OpCode::IDXA: return "IDXA";
        case OpCode::IDXB: return "IDXB";
        case OpCode::IDXC: return "IDXC";
        case OpCode::WRITEAX: return "WRITEAX";
        case OpCode::WRITEBX: return "WRITEBX";
        case OpCode::WRITECX: return "WRITECX";
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::MUL: return "MUL";
        case OpCode::DIV: return "DIV";
        case OpCode::IDIV: return "IDIV";
        case OpCode::MOD: return "MOD";
        case OpCode::EXP: return "EXP";
        case OpCode::LSHIFT: return "LSHIFT";
        case OpCode::RSHIFT: return "RSHIFT";
        case OpCode::BNOT: return "BNOT";
        case OpCode::BAND: return "BAND";
        case OpCode::BOR: return "BOR";
        case OpCode::XOR: return "XOR";
        case OpCode::ATAN: return "ATAN";
        case OpCode::COS: return "COS";
        case OpCode::LOG: return "LOG";
        case OpCode::SIN: return "SIN";
        case OpCode::SQR: return "SQR";
        case OpCode::TAN: return "TAN";
        case OpCode::RND: return "RND";
        case OpCode::SEED: return "SEED";
        case OpCode::FLT: return "FLT";
        case OpCode::INT: return "INT";
        case OpCode::PTR: return "PTR";
        case OpCode::STR: return "STR";
        case OpCode::VSTR: return "VSTR";
        case OpCode::AND: return "AND";
        case OpCode::OR: return "OR";
        case OpCode::NOT: return "NOT";
        case OpCode::EQ: return "EQ";
        case OpCode::NE: return "NE";
        case OpCode::GT: return "GT";
        case OpCode::GE: return "GE";
        case OpCode::LT: return "LT";
        case OpCode::LE: return "LE";
        case OpCode::CMP: return "CMP";
        case OpCode::SETIDX: return "SETIDX";
        case OpCode::MOVIDX: return "MOVIDX";
        case OpCode::LOADIDX: return "LOADIDX";
        case OpCode::STOREIDX: return "STOREIDX";
        case OpCode::INCIDX: return "INCIDX";
        case OpCode::SAVEIDX: return "SAVEIDX";
        case OpCode::PUSHIDX: return "PUSHIDX";
        case OpCode::POPIDX: return "POPIDX";
        case OpCode::JMP: return "JMP";
        case OpCode::JMPEZ: return "JMPEZ";
        case OpCode::JMPNZ: return "JMPNZ";
        case OpCode::IDATA: return "IDATA";
        case OpCode::FDATA: return "FDATA";
        case OpCode::PDATA: return "PDATA";
        case OpCode::SDATA: return "SDATA";
        case OpCode::SYSCALL: return "SYSCALL";
        case OpCode::CALL: return "CALL";
        case OpCode::RETURN: return "RETURN";
        case OpCode::IRQ: return "IRQ";
        case OpCode::ALLOC: return "ALLOC";
        case OpCode::CALLOC: return "CALLOC";
        case OpCode::FREE: return "FREE";
        case OpCode::FREEIDX: return "FREEIDX";
        case OpCode::COPY: return "COPY";
        case OpCode::YIELD: return "YIELD";
        case OpCode::TRACE: return "TRACE";
        default: return "????";
    }
}

void Debugger::debug(OpCode opcode, uint32_t pc, uint8_t sp, uint32_t callstack, value_t a, value_t b, value_t c, vmpointer_t idx, value_t memidx, uint32_t heapidx, std::stack<value_t> stack, std::vector<value_t> mem, std::vector<value_t> heap) {
    std::cerr << "[" << pc << "] " << (int)opcode << ":" << OpCodeAsString(opcode) << " sp: " << (uint32_t)sp << " callstack: " << callstack << " [";

    if (IS_BYTE(a) && 0)
        std::cerr << (uint32_t)ValueAsByte(a) << "b";
    else if (IS_INT(a))
        std::cerr << ValueAsInt(a) << "s";
    else if (IS_POINTER(a))
        std::cerr << "&" << ValueAsPointer(a);
    else
        std::cerr << ValueAsReal(a) << "f";

    std::cerr << ",";

    if (IS_BYTE(b) && 0)
        std::cerr << (uint32_t)ValueAsByte(b) << "b";
    else if (IS_INT(b))
        std::cerr << ValueAsInt(b) << "s";
    else if (IS_POINTER(b))
        std::cerr << "&" << ValueAsPointer(b);
    else
        std::cerr << ValueAsReal(b) << "f";

    std::cerr << ",";

    if (IS_BYTE(c) && 0)
        std::cerr << (uint32_t)ValueAsByte(c) << "b";
    else if (IS_INT(c))
        std::cerr << ValueAsInt(c) << "s";
    else if (IS_POINTER(c))
        std::cerr << "&" << ValueAsPointer(c);
    else
        std::cerr << ValueAsReal(c) << "f";

    std::cerr << "] ";

    std::cerr << "idx: " << ValueAsPointer(idx) << "[";

    if (IS_BYTE(memidx) && 0)
        std::cerr << (uint32_t)ValueAsByte(memidx) << "b";
    else if (IS_INT(memidx))
        std::cerr << ValueAsInt(memidx) << "s";
    else if (IS_POINTER(memidx))
        std::cerr << "&" << ValueAsPointer(memidx);
    else
        std::cerr << ValueAsReal(memidx) << "f";

    std::cerr << "]";

    std::cerr << " heap: " << heapidx;
    std::cerr << " stack: " << stack.size();

    std::cerr << std::endl;
}

Program::Program() {
    entry = 0;
}

Program::Program(const std::vector<uint8_t> &data) {
    entry = 0;
    std::copy(data.begin(), data.end(), back_inserter(code));
}

void Program::addByte(uint8_t b) {
    code.push_back(b);
}

void Program::addShort(int16_t s) {
    uint8_t *bytes = (uint8_t *)&s;

    addByte(bytes[0]);
    addByte(bytes[1]);
}

void Program::addFloat(float f) {
    uint8_t *bytes = (uint8_t *)&f;

    addByte(bytes[0]);
    addByte(bytes[1]);
    addByte(bytes[2]);
    addByte(bytes[3]);
}

void Program::addPointer(vmpointer_t p) {
    uint8_t *bytes = (uint8_t *)&p;

    addByte(bytes[0]);
    addByte(bytes[1]);

#ifdef SYS32
    addByte(bytes[2]);
    addByte((uint8_t)(bytes[3] & 0x07));

#ifdef POINTER64
    addByte(0);
    addByte(0);
    addByte(0);
    addByte(0);
#endif
#else
    addByte((uint8_t)(bytes[2] & 0x7F));
    addByte(0);
#endif
}

void Program::addValue(value_t v) {
    uint8_t *bytes = (uint8_t *)&v;

    addByte(bytes[0]);
    addByte(bytes[1]);
    addByte(bytes[2]);
    addByte(bytes[3]);

#ifdef SYS32
    addByte(bytes[4]);
    addByte(bytes[5]);
    addByte(bytes[6]);
    addByte(bytes[7]);
#endif
}

void Program::addSyscall(SysCall syscall) {
    uint8_t *bytes = (uint8_t *)&syscall;

    addByte(bytes[0]);
    addByte(bytes[1]);
}

uint32_t Program::add(OpCode opcode, const std::string &label) {
    uint32_t pos = code.size();

    addLabel(label, pos);

    addByte((uint8_t)opcode);

    return pos;
}

uint32_t Program::addByte(OpCode opcode, uint8_t b, const std::string &label) {
    uint32_t pos = code.size();

    addLabel(label, pos);

    addByte((uint8_t)opcode);
    addByte(b);

    return pos;
}

uint32_t Program::addShort(OpCode opcode, int16_t s, const std::string &label) {
    uint32_t pos = code.size();

    addLabel(label, pos);

    addByte((uint8_t)opcode);
    addShort(s);

    return pos;
}

uint32_t Program::addFloat(OpCode opcode, float f, const std::string &label) {
    uint32_t pos = code.size();

    addLabel(label, pos);

    addByte((uint8_t)opcode);
    addFloat(f);

    return pos;
}

uint32_t Program::addString(OpCode opcode, const std::string &str, const std::string &label) {
    uint32_t pos = code.size();

    addLabel(label, pos);

    addByte((uint8_t)opcode);    
    for (const auto c : str) 
        addByte((uint8_t)c);
    addByte((uint8_t)0);

    return pos;

}

uint32_t Program::addPointer(OpCode opcode, vmpointer_t p, const std::string &label) {
    uint32_t pos = code.size();

    code.push_back((uint8_t)opcode);
    addPointer(p);

    return pos;
}

uint32_t Program::addValue(OpCode opcode, value_t v, const std::string &label) {
    uint32_t pos = code.size();

    addLabel(label, pos);

    code.push_back((uint8_t)opcode);
    addValue(v);

    return pos;
}

uint32_t Program::addSyscall(OpCode opcode, SysCall syscall, RuntimeValue rtarg, const std::string &label) {
    uint32_t pos = code.size();

    addLabel(label, pos);

    code.push_back((uint8_t)opcode);
    addSyscall(syscall);
    addShort((int16_t)rtarg);

    return pos;
}

uint32_t Program::addLabel(const std::string &label, uint32_t pos) {
    if (label.size()) {
        labels[label] = pos;
    }

    return pos;
}

void Program::updateShort(uint32_t pos, int16_t s) {
    uint8_t *bytes = (uint8_t *)&s;

    code[pos+0] = bytes[0];
    code[pos+1] = bytes[1];
}

void Program::updatePointer(uint32_t pos, vmpointer_t p) {
    uint8_t *bytes = (uint8_t *)&p;

    code[pos+0] = bytes[0];
    code[pos+1] = bytes[1];

#ifdef SYS32
    code[pos+2] = bytes[2];
    code[pos+3] = (bytes[3] & 0x07);

#ifdef POINTER64
    code[pos+4] = 0;
    code[pos+5] = 0;
    code[pos+6] = 0;
    code[pos+7] = 0;
#endif
#else
    code[pos+2] = (bytes[2] & 0x7F);
    code[pos+3] = 0;
#endif
}

void Program::updateValue(uint32_t pos, value_t v) {
    uint8_t *bytes = (uint8_t *)&v;

    code[pos+0] = bytes[0];
    code[pos+1] = bytes[1];
    code[pos+2] = bytes[2];
    code[pos+3] = bytes[3];

#ifdef SYS32
    code[pos+4] = bytes[4];
    code[pos+5] = bytes[5];
    code[pos+6] = bytes[6];
    code[pos+7] = bytes[7];
#endif
}

OpCode Program::fetch(uint32_t pos) const {
    if (pos >= code.size()) {
        return OpCode::HALT;
    }

    const uint8_t *bytes = code.data();
    return (OpCode)bytes[pos];
}

uint8_t Program::readByte(uint32_t pos) const {
    return code[pos];
}

int16_t Program::readShort(uint32_t pos) const {
    const uint8_t *bytes = code.data();

    int16_t s = *((int16_t *)(&bytes[pos]));

    return s;
}

float Program::readFloat(uint32_t pos) const {
    const uint8_t *bytes = code.data();

    float f = *((float *)(&bytes[pos]));

    return f;
}

vmpointer_t Program::readPointer(uint32_t pos) const {
    const uint8_t *bytes = code.data();

    vmpointer_t p = bytes[pos] | (uint32_t)(bytes[pos+1])<<8 | (uint32_t)(bytes[pos+2]) << 16 | (uint32_t)(bytes[pos+3]) << 24;

    return (p & (~(QNAN | SIGN_BIT)));
}

value_t Program::readValue(uint32_t pos) const {
    const uint8_t *bytes = code.data();

    value_t v = *((value_t *)(&bytes[pos]));

    return v;
}

std::string Program::readString(uint32_t pos) const {
    std::string str;
    const uint8_t *bytes = code.data();

    while (char c = bytes[pos++]) {
        str += c;
    }

    return str;
}

void VM::error(const std::string &err) {
    throw std::runtime_error(err);
}

void VM::set(vmpointer_t ptr, value_t v) {
    mem[ptr] = v;
}

uint8_t VM::getByte(vmpointer_t ptr) {
    value_t value = mem[ptr];

    if (!(IS_BYTE(value)))
        error("Value is not a byte");

    return ValueAsByte(value);
}

// TODO
int16_t VM::getShort(vmpointer_t ptr) {
    value_t value = mem[ptr];

    if (!(IS_INT(value)))
        error("Value is not an int");

    return ValueAsInt(value);
}

real_t VM::getReal(vmpointer_t ptr) {
    value_t value = mem[ptr];

    if (!(IS_REAL(value)))
        error("Value is not a float");

    return ValueAsReal(value);
}

vmpointer_t VM::getPointer(vmpointer_t ptr) {
    value_t value = mem[ptr];

    if (!(IS_POINTER(value)))
        error("Value is not a pointer");

    return ValueAsPointer(value);
}

value_t VM::getValue(vmpointer_t ptr) {
    value_t value = mem[ptr];

    return value;
}

std::string VM::getString(vmpointer_t ptr, uint32_t len) {
    std::string str;

    while (ptr < (ptr+len)) {
        char c = (char)getByte(ptr);

        if (!c)
            break;

        str += c;
        ptr++;
    }

    return str;
}

static value_t stringToValue(const std::string &str) {
    value_t value = 0;
    size_t pos;

    if (str.size() > 2 && str[0] == '0' && (str[1] =='x' || str[1] =='X' )) {
        value = PointerAsValue(std::stoul(str, &pos, 16));
    } else if (str.find('.') != std::string::npos) {
        value = RealAsValue(std::stof(str, &pos));
    } else {
        overflow_t overflow = std::stoi(str, &pos);

        if (overflow > _INT_MAX || overflow < _INT_MIN) {
            value = RealAsValue((real_t)overflow);
        } else {
            value = IntAsValue((integer_t)overflow);
        }
    }

    return value;
}

int32_t VM::Syscall(std::shared_ptr<SysIO> sysIO, SysCall syscall, RuntimeValue rvalue, uint32_t cycle_budget) {
    static uint32_t offset = 0;

    static std::shared_ptr<Debugger> tracer = std::make_shared<Debugger>();

    switch(syscall) {
        case SysCall::CLS:
            sysIO->cls();
            break;
        case SysCall::WRITE: {
                if (rvalue == RuntimeValue::PC) {
                    
                } else {
                    value_t arg = getRuntimeValue(rvalue);
                    value_t ptr = arg;

                    while (IS_POINTER(arg)) {
                        ptr = arg;
                        arg = getValue(ValueAsPointer(arg));
                    }

                    if (IS_BYTE(arg)) {
                        if (arg != ptr) {
                            std::string str;
                            while (getByte(ValueAsPointer(ptr))) {
                                //sysIO->write(getByte(ValueAsPointer(ptr++)));
                                str += getByte(ValueAsPointer(ptr++));
                            }
                            sysIO->puts(str);
                        } else {
                            sysIO->puts(std::to_string(ValueAsByte(arg)));
                        }
                    } else if (IS_INT(arg)) {
                        sysIO->puts(std::to_string(ValueAsInt(arg)));
                    } else if (IS_REAL(arg)) {
                        sysIO->puts(std::to_string(ValueAsReal(arg)));
                    }
                }
            }
            break;
        case SysCall::READ: {
                vmpointer_t ptr;

                switch (rvalue) {
                    case RuntimeValue::IDX:
                        ptr = idx;
                        break;
                    default:
                        error("Cannot read to non index register");
                }

                uint8_t chr;
                while ((chr = sysIO->read(false)) != '\n') {
                    --cycle_budget;

                    if (chr == 0)
                        return 0;

                    if (cycle_budget <= 0)
                        return 0;

                    set(ptr+offset, ByteAsValue(chr));
                    offset++;
                }
                set(ptr+offset, ByteAsValue(0));
            }
            break;
        case SysCall::READKEY:
            switch (rvalue) {
                case RuntimeValue::A:
                    a = IntAsValue(sysIO->read(true));
                    break;
                case RuntimeValue::B:
                    b = IntAsValue(sysIO->read(true));
                    break;
                case RuntimeValue::C:
                    c = IntAsValue(sysIO->read(true));
                    break;
                default:
                    error("Cannot readkey to register");
            }
            break;
        case SysCall::KEYSET:
            switch (rvalue) {
                case RuntimeValue::A:
                    c = IntAsValue(sysIO->keyset(ValueAsByte(a)) ? 1 : 0);
                    break;
                case RuntimeValue::B:
                    c = IntAsValue(sysIO->keyset(ValueAsByte(b)) ? 1 : 0);
                    break;
                case RuntimeValue::C:
                    c = IntAsValue(sysIO->keyset(ValueAsByte(c)) ? 1 : 0);
                    break;
                default:
                    error("Cannot readkey to register");
            }
            break;
        case SysCall::CURSOR:
            sysIO->setcursor(ValueAsByte(a), ValueAsByte(b));
            break;
        case SysCall::COLOUR:
            sysIO->setcolours(ValueAsByte(a), ValueAsByte(b));
            break;
        case SysCall::PALETTE:
            switch (rvalue) {
                case RuntimeValue::A:
                    if (IS_BYTE(a))
                        sysIO->palette(ValueAsByte(a));
                    else
                        error("Invalid palette");
                    break;
                case RuntimeValue::B:
                    if (IS_BYTE(b))
                        sysIO->palette(ValueAsByte(b));
                    else
                        error("Invalid palette");
                    break;
                case RuntimeValue::C:
                    if (IS_BYTE(c))
                        sysIO->palette(ValueAsByte(c));
                    else
                        error("Invalid palette");
                    break;
                default:
                    error("Cannot readkey to register");
            }
            break;
        case SysCall::DRAW:
            sysIO->setpixel(ValueAsInt(a), ValueAsInt(b), ValueAsInt(c));
            break;
        case SysCall::DRAWLINE: {
                int x0,y0,x1,y1,colour;

                if (IS_REAL(mem[idx])) {
                    x0 = (int)getReal(idx);
                } else if (IS_INT(mem[idx])) {
                    x0 = (int)getShort(idx);
                } else {
                    error("Invalid type for x0");
                }

                if (IS_REAL(mem[idx+1])) {
                    y0 = (int)getReal(idx+1);
                } else if (IS_INT(mem[idx+1])) {
                    y0 = (int)getShort(idx+1);
                } else {
                    error("Invalid type for y0");
                }

                if (IS_REAL(mem[idx+2])) {
                    x1 = (int)getReal(idx+2);
                } else if (IS_INT(mem[idx+2])) {
                    x1 = (int)getShort(idx+2);
                } else {
                    error("Invalid type for x1");
                }

                if (IS_REAL(mem[idx+3])) {
                    y1 = (int)getReal(idx+3);
                } else if (IS_INT(mem[idx+3])) {
                    y1 = (int)getShort(idx+3);
                } else {
                    error("Invalid type for y1");
                }

                if (IS_REAL(mem[idx+4])) {
                    colour = (int)getReal(idx+4);
                } else if (IS_INT(mem[idx+4])) {
                    colour = (int)getShort(idx+4);
                } else {
                    error("Invalid type for colour");
                }

                int steep = 0;

                if (std::abs(x0-x1) < std::abs(y0-y1)) {
                    std::swap(x0,y0);
                    std::swap(x1,y1);
                    steep = 1;
                }

                if (x0>x1) {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }

                int dx = x1-x0;
                int dy = y1-y0;

                int derror2 = std::abs(dy) * 2;
                int error2 = 0;

                int y = y0;

//std::cerr << "[" << fa << "," << fb << "](" << x0 << "," << y0 << ")-(" << x1 << "," << y1 << ")," << colour << std::endl;

                //tracer->debug(OpCode::SYSCALL, pc, sp, callstack[sp], a, b, c, idx, mem[idx], heap, stack.size());

                for (int x=x0; x <= x1; x++) {
                    if (steep) {
                        sysIO->setpixel(y, x, colour);
                    } else {
                        sysIO->setpixel(x, y, colour);
                    }

                    error2 += derror2;

                    if (error2 > dx) {
                        error2 -= dx*2;

                        if (y1>y0) {
                            y += 1;
                        } else {
                            y -= 1;
                        }
                    }
                }
            }
            break;
        case SysCall::DRAWBOX: {
                int x0,y0,x1,y1,colour,filled;

                if (IS_REAL(mem[idx])) {
                    x0 = (int)getReal(idx);
                } else if (IS_INT(mem[idx])) {
                    x0 = (int)getShort(idx);
                } else {
                    error("Invalid type for x0");
                }

                if (IS_REAL(mem[idx+1])) {
                    y0 = (int)getReal(idx+1);
                } else if (IS_INT(mem[idx+1])) {
                    y0 = (int)getShort(idx+1);
                } else {
                    error("Invalid type for y0");
                }

                if (IS_REAL(mem[idx+2])) {
                    x1 = (int)getReal(idx+2);
                } else if (IS_INT(mem[idx+2])) {
                    x1 = (int)getShort(idx+2);
                } else {
                    error("Invalid type for x1");
                }

                if (IS_REAL(mem[idx+3])) {
                    y1 = (int)getReal(idx+3);
                } else if (IS_INT(mem[idx+3])) {
                    y1 = (int)getShort(idx+3);
                } else {
                    error("Invalid type for y1");
                }

                if (IS_REAL(mem[idx+4])) {
                    colour = (int)getReal(idx+4);
                } else if (IS_INT(mem[idx+4])) {
                    colour = (int)getShort(idx+4);
                } else {
                    error("Invalid type for colour");
                }
                if (IS_REAL(mem[idx+5])) {
                    filled = (int)getReal(idx+5);
                } else if (IS_INT(mem[idx+5])) {
                    filled = (int)getShort(idx+5);
                } else {
                    error("Invalid type for filled");
                }

                if (filled) {
                    for (int y = y0; y <= y1; y++) {
                        for (int x = x0; x <= x1; x++) {
                            sysIO->setpixel(x, y, colour);
                        }
                    }
                } else {
                    for (int x = x0; x <= x1; x++) {
                        sysIO->setpixel(x, y0, colour);
                        sysIO->setpixel(x, y1, colour);
                    }

                    for (int y = y0; y <= y1; y++) {
                        sysIO->setpixel(x0, y, colour);
                        sysIO->setpixel(x1, y, colour);
                    }
                }
            }
            break;
        case SysCall::BLIT: {
                auto x = ValueAsInt(a);
                auto y = ValueAsInt(b);
                auto count = (uint16_t)ValueAsInt(c);
                std::vector<uint8_t> buffer;

                buffer.resize(count);

                auto slice = std::vector<value_t>{mem.begin() + idx, mem.begin() + idx + count};

                std::transform(slice.begin(), slice.end(), buffer.begin(),
                    [](value_t v) {
                        return ValueAsByte(v);
                    }
                );

                sysIO->blit(x, y, buffer);
            }
            break;
        case SysCall::SOUND: {
                float frequency;

                if (IS_INT(a)) {
                     frequency = (float)ValueAsInt(a);
                } else {
                    frequency = (float)ValueAsReal(a);
                }
                uint16_t duration = (uint16_t)ValueAsInt(b);

                sysIO->sound(ValueAsInt(c), frequency, duration);
            }
            break;
        case SysCall::VOICE: {
                uint8_t voice = 0;

                switch (rvalue) {
                    case RuntimeValue::A:
                        voice = ByteAsValue(a);
                        break;
                    case RuntimeValue::B:
                        voice = ByteAsValue(b);
                        break;
                    case RuntimeValue::C:
                        voice = ByteAsValue(c);
                        break;
                    default:
                        error("Cannot read voice index");
                }

                vmpointer_t ptr;

                if (IS_POINTER(mem[idx])) {
                    ptr = getPointer(idx);
                } else {
                    ptr = idx;
                }

                uint8_t waveForm = getShort(ptr);
                uint8_t volume = getShort(ptr+1);
                uint8_t attack = getShort(ptr+2);
                uint8_t decay = getShort(ptr+3);
                uint8_t sustain = getShort(ptr+4);
                uint8_t release = getShort(ptr+5);

                //std::cerr << (int)voice << "," << (int)waveForm << "," << (int)volume << "," << (int)attack << "," << (int)decay << "," << (int)sustain << "," << (int)release << std::endl;

                sysIO->voice(voice, waveForm, volume, attack, decay, sustain, release);

            }
            break;
        case SysCall::MOUSE: {
                int16_t x;
                int16_t y;
                uint16_t buttons;

                sysIO->mousestate(x, y, buttons);

                a = IntAsValue((integer_t)x);
                b = IntAsValue((integer_t)y);
                c = IntAsValue((uint16_t)buttons);
            }
            break;
        default:
            error("Unknown SYSCALL");
    }

    offset = 0;
    return 1;
}

VM::VM(uint32_t _ptrspace) : idx(0),  pc(0), sp(0), ptrspace(_ptrspace) {
    a = IntAsValue(0);
    b = IntAsValue(0);
    c = IntAsValue(0);

    if (ptrspace > mem.max_size()) {
        std::cerr << "Error: memory size is greater than " << mem.max_size() << std::endl;
        exit(-1);
    }

    if (ptrspace > PTR_MASK) {
        std::cerr << "Error: memory size is greater than " << PTR_MASK << std::endl;
        exit(-1);
    }

    callstack.fill(0);

    mem.resize(ptrspace, QNAN);

    heap = mem.size();
}

int VM::compare(vmpointer_t a, vmpointer_t b) {
    if (a == b)
        return 0;

    while (ValueAsInt(mem[a])) {
        if (mem[a] != mem[b])
            break;

        a++;
        b++;
    }

    return ValueAsInt(mem[a]) - ValueAsInt(mem[b]);
}

bool VM::run(std::shared_ptr<SysIO> sysIO, const Program &program, uint32_t cycle_budget, std::shared_ptr<Debugger> debugger) {
    bool done = false;
    uint32_t cycles = 0;

    vmpointer_t p;
    overflow_t overflow;

    std::string str;

    uint32_t offset = 0;

    static int16_t trace = 0;

    static std::shared_ptr<Debugger> tracer = std::make_shared<Debugger>();

    while (!done) {
        uint32_t cost = 1;
        str.clear();

        if (debugger)
            debugger->debug(program.fetch(pc), pc, sp, callstack[sp], a, b, c, idx, mem[idx], heap, stack, {mem.begin(), mem.begin()+48}, {mem.end()-32, mem.end()});

        switch (program.fetch(pc++)) {
            case OpCode::NOP:
                cost = 0;
                break;
            case OpCode::HALT:
                pc = 0;
                done = true;
                break;
            case OpCode::SETA:
                a = program.readValue(pc);
                pc += sizeof(value_t);
                break;
            case OpCode::SETB:
                b = program.readValue(pc);
                pc += sizeof(value_t);
                break;
            case OpCode::SETC:
                c = program.readValue(pc);
                pc += sizeof(value_t);
                break;
            case OpCode::LOADA:
                p = program.readPointer(pc);
                a = getValue(p);
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::LOADB:
                p = program.readPointer(pc);
                b = getValue(p);
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::LOADC:
                p = program.readPointer(pc);
                c = getValue(p);
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::STOREA:
                p = program.readPointer(pc);
                set(p, a);
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::STOREB:
                p = program.readPointer(pc);
                set(p, b);
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::STOREC:
                p = program.readPointer(pc);
                set(p, c);
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::READA:
                if (IS_INT(program.readValue(pc)))
                    p = fp() + ValueAsInt(program.readValue(pc));
                else
                    error("READA is not an integer");
                a = getValue(p);
                pc += sizeof(value_t);
                break;
            case OpCode::READB:
                if (IS_INT(program.readValue(pc)))
                    p = fp() + ValueAsInt(program.readValue(pc));
                else
                    error("READB is not an integer");
                b = getValue(p);
                pc += sizeof(value_t);
                break;
            case OpCode::READC:
                if (IS_INT(program.readValue(pc)))
                    p = fp() + ValueAsInt(program.readValue(pc));
                else
                    error("READC is not an integer");
                c = getValue(p);
                pc += sizeof(value_t);
                break;
            case OpCode::WRITEA:
                if (IS_INT(program.readValue(pc)))
                    p = fp() + ValueAsInt(program.readValue(pc));
                else
                    error("WRITEA is not an integer");
                set(p, a);
                pc += sizeof(value_t);
                break;
            case OpCode::WRITEB:
                if (IS_INT(program.readValue(pc)))
                    p = fp() + ValueAsInt(program.readValue(pc));
                else
                    error("WRITEC is not an integer");
                set(p, b);
                pc += sizeof(value_t);
                break;
            case OpCode::WRITEC:
                if (IS_INT(program.readValue(pc)))
                    p = fp() + ValueAsInt(program.readValue(pc));
                else
                    error("WRITEC is not an integer");
                set(p, c);
                pc += sizeof(value_t);
                break;
            case OpCode::PUSHA:
                stack.push(a);
                break;
            case OpCode::PUSHB:
                stack.push(b);
                break;
            case OpCode::PUSHC:
                stack.push(c);
                break;
            case OpCode::POPA:
                a = stack.top();
                stack.pop();
                break;
            case OpCode::POPB:
                b = stack.top();
                stack.pop();
                break;
            case OpCode::POPC:
                c = stack.top();
                stack.pop();
                break;
            case OpCode::MOVCA:
                a = c;
                break;
            case OpCode::MOVCB:
                b = c;
                break;
            case OpCode::MOVCIDX:
                if (IS_POINTER(c))
                    idx = ValueAsPointer(c);
                else if (IS_INT(c))
                    idx = (vmpointer_t)ValueAsInt(c);
                else
                    error("MOVCIDX is not a pointer");
                break;
            case OpCode::INCA:
                if (IS_INT(a) && IS_INT(program.readValue(pc)))
                    a = IntAsValue(ValueAsInt(a) + ValueAsInt(program.readValue(pc)));
                else if (IS_REAL(a) && IS_REAL(program.readValue(pc)))
                    a = RealAsValue(ValueAsReal(a) + ValueAsReal(program.readValue(pc)));
                else if (IS_POINTER(a) && IS_INT(program.readValue(pc)))
                    a = PointerAsValue(ValueAsPointer(a) + ValueAsInt(program.readValue(pc)));
                else
                    error("INCA mismatch");
                pc += sizeof(value_t);
                break;
            case OpCode::INCB:
                if (IS_INT(b) && IS_INT(program.readValue(pc)))
                    b = IntAsValue(ValueAsInt(b) + ValueAsInt(program.readValue(pc)));
                else if (IS_REAL(b) && IS_REAL(program.readValue(pc)))
                    b = RealAsValue(ValueAsReal(b) + ValueAsReal(program.readValue(pc)));
                else if (IS_POINTER(b) && IS_INT(program.readValue(pc)))
                    b = PointerAsValue(ValueAsPointer(b) + ValueAsInt(program.readValue(pc)));
                else
                    error("INCB mismatch");
                pc += sizeof(value_t);
                break;
            case OpCode::INCC:
                if (IS_INT(c) && IS_INT(program.readValue(pc)))
                    c = IntAsValue(ValueAsInt(c) + ValueAsInt(program.readValue(pc)));
                else if (IS_REAL(c) && IS_REAL(program.readValue(pc)))
                    c = RealAsValue(ValueAsReal(c) + ValueAsReal(program.readValue(pc)));
                else if (IS_POINTER(c) && IS_INT(program.readValue(pc)))
                    c = PointerAsValue(ValueAsPointer(c) + ValueAsInt(program.readValue(pc)));
                else
                    error("INCC mismatch");
                pc += sizeof(value_t);
                break;
            case OpCode::IDXA:
                a = getValue(idx);
                break;
            case OpCode::IDXB:
                b = getValue(idx);
                break;
            case OpCode::IDXC:
                c = getValue(idx);
                break;
            case OpCode::WRITEAX:
                set(idx, a);
                break;
            case OpCode::WRITEBX:
                set(idx, b);
                break;
            case OpCode::WRITECX:
                set(idx, c);
                break;
            case OpCode::ADD:
                if (IS_INT(a) && IS_INT(b)) {
                    overflow = (overflow_t)ValueAsInt(a) + (overflow_t)ValueAsInt(b);
                    if (overflow > _INT_MAX || overflow < _INT_MIN) {
                        //error(std::string("ADD overflow: ") + std::to_string(overflow));
                        //std::cerr << std::string("ADD overflow: ") << std::to_string(overflow) << "=" << ValueAsInt(a) << "+" << ValueAsInt(b) << std::endl;
                        c = RealAsValue((real_t)overflow);
                    } else 
                        c = IntAsValue(overflow);
                }
                else if (IS_REAL(a) && IS_REAL(b))
                    c = RealAsValue(ValueAsReal(a) + ValueAsReal(b));
                else if (IS_REAL(a) && IS_INT(b))
                    c = RealAsValue(ValueAsReal(a) + (real_t)ValueAsInt(b));
                else if (IS_INT(a) && IS_REAL(b))
                    c = RealAsValue((real_t)ValueAsInt(a) + ValueAsReal(b));
                else if (IS_POINTER(a) && IS_INT(b))
                    c = PointerAsValue(ValueAsPointer(a) + ValueAsInt(b));
                else if (IS_INT(a) && IS_POINTER(b))
                    c = PointerAsValue(ValueAsPointer(b) + ValueAsInt(a));
                else if (IS_POINTER(a) && IS_REAL(b))
                    c = PointerAsValue(ValueAsPointer(a) + (uint16_t)ValueAsReal(b));
                else if (IS_REAL(a) && IS_POINTER(b))
                    c = PointerAsValue(ValueAsPointer(b) + (uint16_t)ValueAsReal(a));
                else
                    error("ADD mismatch");
                break;
            case OpCode::SUB:
                if (IS_INT(a) && IS_INT(b)) {
                    overflow = ValueAsInt(a) - ValueAsInt(b);
                    if (overflow > _INT_MAX || overflow < _INT_MIN) {
                        //std::cerr << std::string("SUB overflow: ") << std::to_string(overflow) << "=" << ValueAsInt(a) << "-" << ValueAsInt(b) << std::endl;
                        c = RealAsValue((real_t)overflow);
                    } else 
                        c = IntAsValue(overflow);
                }
                else if (IS_REAL(a) && IS_REAL(b))
                    c = RealAsValue(ValueAsReal(a) - ValueAsReal(b));
                else if (IS_REAL(a) && IS_INT(b))
                    c = RealAsValue(ValueAsReal(a) - (real_t)ValueAsInt(b));
                else if (IS_INT(a) && IS_REAL(b))
                    c = RealAsValue((real_t)ValueAsInt(a) - ValueAsReal(b));
                else if (IS_POINTER(a) && IS_INT(b))
                    c = PointerAsValue(ValueAsPointer(a) - ValueAsInt(b));
                else if (IS_INT(a) && IS_POINTER(b))
                    c = PointerAsValue(ValueAsPointer(b) - ValueAsInt(a));
                else if (IS_POINTER(a) && IS_REAL(b))
                    c = PointerAsValue(ValueAsPointer(a) - (uint16_t)ValueAsReal(b));
                else if (IS_REAL(a) && IS_POINTER(b))
                    c = PointerAsValue(ValueAsPointer(b) - (uint16_t)ValueAsReal(a));
                else if (IS_POINTER(a) && IS_POINTER(b))
                    c = IntAsValue(std::abs((overflow_t)ValueAsPointer(a) - (overflow_t)ValueAsPointer(b)));
                else
                    error("SUB mismatch");
                break;
            case OpCode::MUL:
                if (IS_INT(a) && IS_INT(b)) {
                    overflow = (overflow_t)ValueAsInt(a) * (overflow_t)ValueAsInt(b);

                    if (overflow > _INT_MAX || overflow < _INT_MIN) {
                        //error(std::string("MUL overflow: ") + std::to_string(overflow));
                        //std::cerr << std::string("MUL overflow: ") << std::to_string(overflow) << "=" << ValueAsInt(a) << "x" << ValueAsInt(b) << std::endl;
                        c = RealAsValue((real_t)overflow);
                    } else
                        c = IntAsValue(overflow);
                }
                else if (IS_REAL(a) && IS_REAL(b))
                    c = RealAsValue(ValueAsReal(a) * ValueAsReal(b));
                else if (IS_REAL(a) && IS_INT(b))
                    c = RealAsValue(ValueAsReal(a) * (real_t)ValueAsInt(b));
                else if (IS_INT(a) && IS_REAL(b))
                    c = RealAsValue((real_t)ValueAsInt(a) * ValueAsReal(b));
                else
                    error("MUL mismatch");
                break;
            case OpCode::DIV:
                if (IS_INT(a) && IS_INT(b))
                    c = RealAsValue((real_t)ValueAsInt(a) / (real_t)ValueAsInt(b));
                else if (IS_REAL(a) && IS_REAL(b))
                    c = RealAsValue(ValueAsReal(a) / ValueAsReal(b));
                else if (IS_REAL(a) && IS_INT(b))
                    c = RealAsValue(ValueAsReal(a) / (real_t)ValueAsInt(b));
                else if (IS_INT(a) && IS_REAL(b))
                    c = RealAsValue((real_t)ValueAsInt(a) / ValueAsReal(b));
                else
                    error("DIV mismatch");
                break;
            case OpCode::IDIV:
                if (IS_INT(a) && IS_INT(b)) {
                    overflow = ValueAsInt(a) / ValueAsInt(b);
                    if (overflow > _INT_MAX || overflow < _INT_MIN)
                        error("IDIV overflow");
                    else
                        c = IntAsValue(overflow);
                }
                else
                    error("IDIV mismatch");
                break;
            case OpCode::MOD:
                if (IS_INT(a) && IS_INT(b))
                    c = IntAsValue(ValueAsInt(a) % ValueAsInt(b));
                else
                    error("MOD mismatch");
                break;
            case OpCode::EXP:
                if (IS_INT(a) && IS_INT(b)) {
                    overflow = std::pow(ValueAsInt(a), ValueAsInt(b));

                    if (overflow > _INT_MAX || overflow < _INT_MIN) {
                        //std::cerr << "EXP overflow " << overflow << std::endl;
                        c = RealAsValue((real_t)overflow);
                    } else
                        c = IntAsValue(overflow);
                }
                else if (IS_REAL(a) && IS_REAL(b))
                    c = RealAsValue(std::pow(ValueAsReal(a), ValueAsReal(b)));
                else if (IS_REAL(a) && IS_INT(b))
                    c = RealAsValue(std::pow(ValueAsReal(a), ValueAsInt(b)));
                else if (IS_INT(a) && IS_REAL(b))
                    c = RealAsValue(std::pow(ValueAsInt(a), ValueAsReal(b)));
                else
                    error("EXP mismatch");
                break;
            case OpCode::LSHIFT:
                if (IS_INT(a) && IS_INT(b))
                    c = IntAsValue(ValueAsInt(a) << ValueAsInt(b));
                else
                    error("LSHIFT mismatch");
                break;
            case OpCode::RSHIFT:
                if (IS_INT(a) && IS_INT(b))
                    c = IntAsValue(ValueAsInt(a) >> ValueAsInt(b));
                else
                    error("RSHIFT mismatch");
                break;
            case OpCode::BNOT:
                if (IS_INT(c))
                    c = IntAsValue(~(ValueAsInt(c)));
                else
                    error("BNOT mismatch");
                break;
            case OpCode::BAND:
                if (IS_INT(a) && IS_INT(b))
                    c = IntAsValue(ValueAsInt(a) & ValueAsInt(b));
                else
                    error("BAND mismatch");
                break;
            case OpCode::BOR:
                if (IS_INT(a) && IS_INT(b))
                    c = IntAsValue(ValueAsInt(a) | ValueAsInt(b));
                else
                    error("BOR mismatch");
                break;
            case OpCode::XOR:
                if (IS_INT(a) && IS_INT(b))
                    c = IntAsValue(ValueAsInt(a) ^ ValueAsInt(b));
                else
                    error("XOR mismatch");
                break;
            case OpCode::ATAN:
                if (IS_INT(c))
                    c = RealAsValue(std::atan((real_t)ValueAsInt(c)));
                else if (IS_REAL(c))
                    c = RealAsValue(std::atan(ValueAsReal(c)));
                else
                    error("ATAN argument error");
                break;
            case OpCode::COS:
                if (IS_INT(c))
                    c = RealAsValue(std::cos((real_t)ValueAsInt(c)));
                else if (IS_REAL(c))
                    c = RealAsValue(std::cos(ValueAsReal(c)));
                else
                    error("COS argument error");
                break;
            case OpCode::LOG:
                if (IS_INT(c))
                    c = RealAsValue(std::log((real_t)ValueAsInt(c)));
                else if (IS_REAL(c))
                    c = RealAsValue(std::log(ValueAsReal(c)));
                else
                    error("LOG argument error");
                break;
            case OpCode::SIN:
                if (IS_INT(c))
                    c = RealAsValue(std::sin((real_t)ValueAsInt(c)));
                else if (IS_REAL(c))
                    c = RealAsValue(std::sin(ValueAsReal(c)));
                else
                    error("SIN argument error");
                break;
            case OpCode::SQR:
                if (IS_INT(c))
                    c = RealAsValue(std::sqrt((real_t)ValueAsInt(c)));
                else if (IS_REAL(c))
                    c = RealAsValue(std::sqrt(ValueAsReal(c)));
                else
                    error("SQR argument error");
                break;
            case OpCode::TAN:
                if (IS_INT(c))
                    c = RealAsValue(std::tan((real_t)ValueAsInt(c)));
                else if (IS_REAL(c))
                    c = RealAsValue(std::tan(ValueAsReal(c)));
                else
                    error("TAN argument error");
                break;
            case OpCode::RND:
                if (IS_INT(c))
                    c = RealAsValue(((real_t)std::rand()/(real_t)RAND_MAX) * (real_t)ValueAsInt(c));
                else if (IS_REAL(c))
                    c = RealAsValue(((real_t)std::rand()/(real_t)RAND_MAX) * (real_t)ValueAsReal(c));
                else
                    error("RND argument error");
                break;
            case OpCode::SEED:
                if (IS_INT(c))
                    std::srand((uint32_t)ValueAsInt(c));
                else if (IS_REAL(c))
                    std::srand((uint32_t)ValueAsReal(c));
                else
                    error("RND argument error");
                break;
            case OpCode::FLT:
                if (IS_INT(c))
                    c = RealAsValue((real_t)ValueAsInt(c));
                else if (IS_REAL(c))
                    c = RealAsValue(ValueAsReal(c));
                else
                    error("FLT argument error");
                break;
            case OpCode::INT:
                if (IS_INT(c)) {
                    c = IntAsValue(c);
                }
                else if (IS_REAL(c)) {
                    overflow = (overflow_t)ValueAsReal(c);

                    if (overflow > _INT_MAX) {
                        //std::cerr << (std::string("INT overflow") + std::to_string(overflow)) << std::endl;
                        c = IntAsValue(_INT_MAX);
                    } else if (overflow < _INT_MIN) {
                        //error(std::string("INT overflow (f)") + std::to_string(overflow));
                        //std::cerr << (std::string("INT underflow") + std::to_string(overflow)) << std::endl;
                        c = IntAsValue(_INT_MIN);
                    } else
                        c = IntAsValue(overflow);
                } else if (IS_POINTER(c))
                    c = IntAsValue((integer_t)ValueAsPointer(c));
                else
                    error("INT argument error");
                break;
            case OpCode::PTR:
                if (IS_INT(c))
                    c = PointerAsValue((vmpointer_t)ValueAsInt(c));
                else if (IS_POINTER(c))
                    c = PointerAsValue(ValueAsPointer(c));
                else
                    error("PTR argument error");
                break;
            case OpCode::STR:
                if (IS_INT(c))
                    str = std::to_string(ValueAsInt(c));
                else if (IS_POINTER(c))
                    str = std::to_string(ValueAsPointer(c));
                else if (IS_REAL(c))
                    str = std::to_string(ValueAsReal(c));
                else
                    error("STR argument error");
                for (size_t i = 0; i < str.size(); i++) {
                    set(idx+i, ByteAsValue(str[i]));
                }
                set(idx+str.size(), ByteAsValue(0));
                break;
            case OpCode::VSTR:
                offset = 0;
                while (char chr = getByte(idx+offset)) {
                    str += chr;
                    offset++;
                }
                c = stringToValue(str);
                break;
            case OpCode::AND:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) && ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) && ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else
                    error("AND mismatch");
                break;
            case OpCode::OR:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) || ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) || ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else
                    error("OR mismatch");
                break;
            case OpCode::NOT:
                if (IS_INT(c))
                    c = IntAsValue(!ValueAsInt(c));
                else if (IS_REAL(c))
                    c = RealAsValue(!ValueAsReal(c));
                else
                    error("NOT mismatch");
                break;
            case OpCode::EQ:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) == ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) == ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_POINTER(a) && IS_POINTER(b))
                    c = ValueAsPointer(a) == ValueAsPointer(b) ? IntAsValue(1) : IntAsValue(0);
                else
                    error("EQ mismatch");
                break;
            case OpCode::NE:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) != ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) != ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_POINTER(a) && IS_POINTER(b))
                    c = compare(ValueAsPointer(a), ValueAsPointer(b)) != 0 ? IntAsValue(1) : IntAsValue(0);
                else
                    error("NE mismatch");
                break;
            case OpCode::GT:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) > ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) > ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_POINTER(a) && IS_POINTER(b))
                    c = compare(ValueAsPointer(a), ValueAsPointer(b)) > 0 ? IntAsValue(1) : IntAsValue(0);
                else
                    error("GT mismatch");
                break;
            case OpCode::GE:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) >= ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) >= ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_POINTER(a) && IS_POINTER(b))
                    c = compare(ValueAsPointer(a), ValueAsPointer(b)) >= 0 ? IntAsValue(1) : IntAsValue(0);
                else
                    error("GE mismatch");
                break;
            case OpCode::LT:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) < ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) < ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_POINTER(a) && IS_POINTER(b))
                    c = compare(ValueAsPointer(a), ValueAsPointer(b)) < 0 ? IntAsValue(1) : IntAsValue(0);
                else
                    error("LT mismatch");
                break;
            case OpCode::LE:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) <= ValueAsInt(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) <= ValueAsReal(b) ? IntAsValue(1) : IntAsValue(0);
                else if (IS_POINTER(a) && IS_POINTER(b))
                    c = compare(ValueAsPointer(a), ValueAsPointer(b)) <= 0 ? IntAsValue(1) : IntAsValue(0);
                else
                    error("LE mismatch");
                break;
            case OpCode::CMP:
                if (IS_INT(a) && IS_INT(b))
                    c = ValueAsInt(a) > ValueAsInt(b) ? IntAsValue(1) : ValueAsInt(a) < ValueAsInt(b) ? IntAsValue(-1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_REAL(b))
                    c = ValueAsReal(a) > ValueAsReal(b) ? IntAsValue(1) : ValueAsReal(a) < ValueAsReal(b) ? IntAsValue(-1) : IntAsValue(0);
                else if (IS_REAL(a) && IS_INT(b))
                    c = ValueAsReal(a) > ValueAsInt(b) ? IntAsValue(1) : ValueAsReal(a) < ValueAsInt(b) ? IntAsValue(-1) : IntAsValue(0);
                else if (IS_INT(a) && IS_REAL(b))
                    c = ValueAsInt(a) > ValueAsReal(b) ? IntAsValue(1) : ValueAsInt(a) < ValueAsReal(b) ? IntAsValue(-1) : IntAsValue(0);
                else if (IS_POINTER(a) && IS_POINTER(b)) {
                    auto valueA = getValue(ValueAsPointer(a));
                    auto valueB = getValue(ValueAsPointer(b));

                    if (IS_INT(valueA) && IS_INT(valueB))
                        c = ValueAsInt(valueA) > ValueAsInt(valueB) ? IntAsValue(1) : ValueAsInt(valueA) < ValueAsInt(valueB) ? IntAsValue(-1) : IntAsValue(0);
                    else if (IS_REAL(valueA) && IS_REAL(valueB))
                        c = ValueAsReal(valueA) > ValueAsReal(valueB) ? IntAsValue(1) : ValueAsReal(valueA) < ValueAsReal(valueB) ? IntAsValue(-1) : IntAsValue(0);
                    else if (IS_REAL(valueA) && IS_INT(valueB))
                        c = ValueAsReal(valueA) > ValueAsInt(valueB) ? IntAsValue(1) : ValueAsReal(valueA) < ValueAsInt(valueB) ? IntAsValue(-1) : IntAsValue(0);
                    else if (IS_INT(valueA) && IS_REAL(valueB))
                        c = ValueAsInt(valueA) > ValueAsReal(valueB) ? IntAsValue(1) : ValueAsInt(valueA) < ValueAsReal(valueB) ? IntAsValue(-1) : IntAsValue(0);
                    else
                        error("CMP PTR mismatch");
                } else
                    error("CMP mismatch");
                break;
            case OpCode::SETIDX:
                idx = program.readPointer(pc);
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::MOVIDX:
                if (IS_INT(program.readValue(pc)))
                    idx = fp() + ValueAsInt(program.readValue(pc));
                else
                    error("MOVIDX value is not an integer");
                pc += sizeof(value_t);
                break;
            case OpCode::LOADIDX:
                idx = getPointer(program.readPointer(pc));
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::STOREIDX:
                set(idx, program.readValue(pc));
                pc += sizeof(value_t);
                break;
            case OpCode::INCIDX:
                idx += ValueAsInt(program.readValue(pc));
                pc += sizeof(value_t);
                break;
            case OpCode::SAVEIDX:
                set(program.readPointer(pc), PointerAsValue(idx));
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::PUSHIDX:
                stack.push(PointerAsValue(idx));
                break;
            case OpCode::POPIDX:
                if (!IS_POINTER(stack.top()))
                    error("Stack value is not a pointer");
                idx = ValueAsPointer(stack.top());
                stack.pop();
                break;
            case OpCode::JMP:
                pc = (uint16_t)program.readShort(pc);
                break;
            case OpCode::JMPEZ:
                if (c == IntAsValue(0))
                    pc = (uint16_t)program.readShort(pc);
                else
                    pc += sizeof(int16_t);
                break;
            case OpCode::JMPNZ:
                if (c != IntAsValue(0))
                    pc = (uint16_t)program.readShort(pc);
                else
                    pc += sizeof(int16_t);
                break;
            case OpCode::IDATA:
                set(idx, IntAsValue(program.readShort(pc)));
                pc += sizeof(int16_t);
                break;
            case OpCode::FDATA:
                set(idx, RealAsValue(program.readFloat(pc)));
                pc += sizeof(value_t);
                break;
            case OpCode::PDATA:
                set(idx, PointerAsValue(program.readPointer(pc)));
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::SDATA:
                offset = 0;
                while (uint8_t b = program.readByte(pc)) {
                    set(idx+offset, IntAsValue(b));
                    pc += 1;
                    offset += 1;
                }
                set(idx+offset, IntAsValue(0));
                pc += 1;
                break;
            case OpCode::SYSCALL:
                if (Syscall(sysIO, (SysCall)program.readShort(pc), (RuntimeValue)program.readShort(pc+2), cycle_budget - cycles))
                    pc += sizeof(int16_t) + sizeof(int16_t);
                else
                    pc -= 1;
                break;
            case OpCode::CALL:
                callstack[++sp] = pc + sizeof(int16_t);
                pc = (uint16_t)program.readShort(pc);
                break;
            case OpCode::RETURN:
                if (sp == 0)
                    error("RETURN without CALL");
                pc = callstack[sp--];
                break;
            case OpCode::IRQ: {
                    auto signal = program.readShort(pc);
                    auto interupt = interupts.find(signal);

                    if (interupt == interupts.end()) {
                        error(std::string("Unknown signal "));
                    } else {
                        pc += sizeof(int16_t);
                        interupt->second(this);
                    }
                }
                break;
            case OpCode::ALLOC:
                heap = HeapAlloc((uint16_t)program.readShort(pc));
                idx = heap;
                pc += sizeof(int16_t);
                break;
            case OpCode::CALLOC:
                if (IS_INT(c))
                    heap = HeapAlloc((uint16_t)ValueAsInt(c));
                else if (IS_REAL(c))
                    heap = HeapAlloc((uint16_t)ValueAsReal(c));
                else
                    error("CALLOC is not a number");
                idx = heap;
                break;
            case OpCode::FREE:
                HeapFree(PointerAsValue(program.readPointer(pc)));
                pc += sizeof(vmpointer_t);
                break;
            case OpCode::FREEIDX:
                HeapFree(idx);
                break;
            case OpCode::COPY:
                MemCopy(ValueAsPointer(a), ValueAsPointer(b), IntAsValue(c));
                break;
            case OpCode::YIELD:
                //std::cerr << "Yield " << cycles << std::endl;
                return done;
                break;
            case OpCode::TRACE:
                trace = program.readShort(pc);
                pc += sizeof(int16_t);

                if (trace) {
                    debugger = tracer;
                } else {
                    if (debugger == tracer)
                        debugger = nullptr;
                }

                break;
            default:
                std::cout << "Unknown opcode " << (int)program.fetch(pc-1)  << " " << (int)(pc-1) << std::endl;
        }

        cycles += cost;

        if (cycles >= cycle_budget) {
            std::cerr << "Budget " << cycles << std::endl;
            break;
        }
    }

    if (done) {
        idx = 0;
        pc = 0;
        sp = 0;
        a = IntAsValue(0);
        b = IntAsValue(0);
        c = IntAsValue(0);

        callstack.fill(0);

        std::fill(mem.begin(), mem.end(), QNAN);

        heap = mem.size();
    }

    return done;
}

vmpointer_t VM::HeapAlloc(uint16_t size) {
    vmpointer_t ptr = heap - size;

    allocList.emplace(ptr, size);

    for (size_t i = 0; i < size; i++) {
        mem[i+ptr] = IntAsValue(0);
    }

    return ptr;
}

void VM::HeapFree(vmpointer_t ptr) {
    for (auto it = allocList.cbegin(); it != allocList.cend(); /**/) {
        if (it->first <= heap) {
            heap += it->second;
            allocList.erase(it++);
        } else {
            ++it;
        }
    }
}

void VM::MemCopy(vmpointer_t dst, vmpointer_t src, integer_t count) {
    for (integer_t i = 0; i < count; i++) {
        mem[dst+i] = mem[src+i];
    }
}

VM::~VM() {
}
