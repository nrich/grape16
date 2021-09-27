#ifndef __EMULATOR_VM_H__
#define __EMULATOR_VM_H__

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <memory>
#include <functional>

#define SIGN_BIT    ((uint32_t)0x80000000)
#define QNAN        ((uint32_t)0x7F800000)
#define BYTEVAL     ((uint32_t)0xFFFF00FF)

#define IS_INT(value)               (((value & SIGN_BIT) != SIGN_BIT) && ((value & QNAN) == QNAN))
#define IS_BYTE(value)              (IS_INT(value) && ((value & BYTEVAL) == value))
#define IS_FLOAT(value)             (((value) & QNAN) != QNAN)
#define IS_POINTER(value)           (((value) & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT))

namespace Emulator {
    typedef uint32_t vmpointer_t;
    typedef uint32_t value_t;

    inline value_t IntAsValue(int16_t s) {
        return (QNAN | (uint16_t)s);
    }

    inline int16_t ValueAsInt(value_t value) {
        if (!IS_INT(value)) {
            std::cerr << "Value is not an int!" << std::endl;
            exit(-1);
        }
        return (int16_t)(0x0000FFFF & value);
    }

    inline value_t ByteAsValue(uint8_t b) {
        return (QNAN | b);
    }

    inline uint8_t ValueAsByte(value_t value) {
        return (uint8_t)(0x000000FF & value);
    }

    inline value_t PointerAsValue(vmpointer_t p) {
        return (QNAN | SIGN_BIT | (value_t)p);
    }

    inline vmpointer_t ValueAsPointer(value_t value) {
        return (vmpointer_t)((~(QNAN | SIGN_BIT)) & value);
    }

    inline value_t FloatAsValue(float f) {
        value_t value;

        memcpy(&value, &f, sizeof(float));

        return value;
    }

    inline float ValueAsFloat(value_t value) {
        float f;

        memcpy(&f, &value, sizeof(float));

        return f;
    }

    inline std::string ValueToString(value_t value) {
        if (IS_INT(value))
            return std::to_string(ValueAsInt(value));
        if (IS_FLOAT(value))
            return std::to_string(ValueAsFloat(value));
        if (IS_POINTER(value))
            return std::to_string(ValueAsPointer(value));

        return "";
    }

    enum class OpCode {
        NOP = 0,

        HALT,

        SETA,
        SETB,
        SETC,

        LOADA,
        LOADB,
        LOADC,

        STOREA,
        STOREB,
        STOREC,

        PUSHA,
        PUSHB,
        PUSHC,

        POPA,
        POPB,
        POPC,

        MOVCA,
        MOVCB,
        MOVCIDX,

        INCA,
        INCB,
        INCC,

        IDXA,
        IDXB,
        IDXC,

        WRITEAX,
        WRITEBX,
        WRITECX,

        ADD,
        SUB,
        MUL,
        DIV,
        IDIV,
        MOD,
        EXP,

        ATAN,
        COS,
        LOG,
        SIN,
        SQR,
        TAN,

        FLT,
        INT,
        PTR,

        AND,
        OR,
        NOT,

        EQ,
        NE,
        GT,
        GE,
        LT,
        LE,
        CMP,

        SETIDX,
        LOADIDX,
        STOREIDX,
        INCIDX,
        SAVEIDX,
        PUSHIDX,
        POPIDX,

        JMP,
        JMPEZ,
        JMPNZ,

        IDATA,
        FDATA,
        PDATA,
        SDATA,

        SYSCALL,

        CALL,
        RETURN,

        IRQ,

        ALLOC,
        CALLOC,

        YIELD,

        TRACE,

        COUNT
    };

    enum class RuntimeValue {
        NONE = 0,
        A,
        B,
        C,
        IDX,
        PC,
        COUNT
    }; 

    enum class SysCall {
        CLS = 0,
        WRITE,
        READ,
        READKEY,
        DRAW,
        DRAWLINE,
        BLIT,
        SOUND,
        COUNT
    };


    std::string OpCodeAsString(OpCode opcode);

    class SysIO {
        public:
            virtual void cls() = 0;

            virtual void write(const uint8_t c) = 0;
            virtual uint8_t read(bool noecho) = 0;

            virtual void puts(const std::string &str) = 0;
            virtual std::string gets() = 0; 

            virtual void setpixel(uint16_t x, uint16_t y, uint8_t pixel) = 0;
            virtual uint8_t getpixel(uint16_t x, uint16_t y) = 0;

            virtual void blit(uint16_t x, uint16_t y, std::vector<uint8_t> buffer) = 0;

            virtual void sound(float frequency, uint16_t duration) = 0;
    };

    class Debugger {
        public:
            virtual void debug(OpCode opcode, uint32_t pc, uint8_t sp, uint32_t callstack, value_t a, value_t b, value_t c, vmpointer_t idx, value_t memidx, uint32_t heap, size_t stack);
    };

    class Program {
        private:
            uint32_t entry;
            std::vector<uint8_t> code;
            std::map<const std::string, uint32_t> labels;
            std::map<const std::string, uint32_t> globals;
            uint32_t datacells;

            void addByte(uint8_t b);
            void addShort(int16_t s);
            void addFloat(float f);
            void addPointer(vmpointer_t p);
            void addValue(value_t v);
            void addSyscall(SysCall syscall);
        public:
            Program();
            Program(const std::vector<uint8_t> &data);
            Program(const Program &) = default;

            uint32_t addLabel(const std::string &label, uint32_t pos);
            uint32_t addLabel(const std::string &label) {
                return addLabel(label, code.size());
            }

            uint32_t addLabel(uint32_t num, uint32_t pos) {
                return addLabel(std::to_string(num), pos);
            }

            uint32_t addLabel(uint32_t num) {
                return addLabel(std::to_string(num), code.size());
            }

            uint32_t getLabel(const std::string &label) {
                return labels.at(label);
            }

            uint32_t getLabel(uint32_t num) {
                return labels.at(std::to_string(num));
            }

            uint32_t add(OpCode opcode, const std::string &label="");
            uint32_t addByte(OpCode opcode, uint8_t b, const std::string &label="");
            uint32_t addShort(OpCode opcode, int16_t s, const std::string &label="");
            uint32_t addFloat(OpCode opcode, float f, const std::string &label="");
            uint32_t addString(OpCode opcode, const std::string &str, const std::string &label="");
            uint32_t addPointer(OpCode opcode, vmpointer_t p, const std::string &label="");
            uint32_t addValue(OpCode opcode, value_t v, const std::string &label="");
            uint32_t addSyscall(OpCode opcode, SysCall syscall, RuntimeValue rtarg, const std::string &label="");

            OpCode fetch(uint32_t pos) const;

            uint8_t readByte(uint32_t pos) const;
            int16_t readShort(uint32_t pos) const;
            float readFloat(uint32_t pos) const;
            std::string readString(uint32_t pos) const;
            vmpointer_t readPointer(uint32_t pos) const;
            value_t readValue(uint32_t pos) const;
            SysCall readSyscall(uint32_t pos) const;

            void setEntryPoint(uint32_t _entry) {
                entry = _entry;
            }

            uint32_t Entry() const {
                return entry;
            }

            uint32_t Global(const std::string &name) {
                if (globals.find(name) == globals.end()) {
                    globals[name] = globals.size() + datacells;
                }

                return globals.at(name);
            }

            uint32_t reserveDataCell() {
                if (globals.size()) {
                    throw std::invalid_argument("Tring to add a data cell after globals set");
                }

                return datacells++;
            }

            uint32_t DataCells() const {
                return datacells;
            }

            uint32_t locals() const {
                return datacells + globals.size();
            }

            void update(uint32_t pos, int16_t s);
            void update(uint32_t pos, OpCode opcode) {
                code[pos] = (uint8_t)opcode;
            }

            size_t size() const {
                return code.size();
            }
    };

    class VM {
        private:
            value_t a;
            value_t b;
            value_t c;

            vmpointer_t idx;

            uint32_t pc;

            uint32_t heap;

            uint8_t sp;
            std::array<uint32_t, 256> callstack;

            std::vector<value_t> mem;

            const uint32_t ptrspace;

            std::map<uint32_t, std::function<void(VM*)>> interupts;

            std::stack<value_t> stack;

            void error(const std::string &err);

            void set(vmpointer_t ptr, value_t v);

            int compare(vmpointer_t a, vmpointer_t b);

            uint8_t getByte(vmpointer_t ptr);
            int16_t getShort(vmpointer_t ptr);
            float getFloat(vmpointer_t prt);
            vmpointer_t getPointer(vmpointer_t ptr);
            std::string getString(vmpointer_t ptr, uint32_t len);

            value_t getValue(vmpointer_t ptr);

            int32_t Syscall(std::shared_ptr<SysIO> sysIO, SysCall syscall, RuntimeValue rvalue, uint32_t cycle_budget);

            value_t getRuntimeValue(RuntimeValue rtarg) const {
                switch(rtarg) {
                    case RuntimeValue::A:
                        return a;
                    case RuntimeValue::B:
                        return b;
                    case RuntimeValue::C:
                        return c;
                    case RuntimeValue::IDX:
                        return PointerAsValue(idx);
                    default:
                        return IntAsValue(0);
                }
            }

        public:
            VM(const uint32_t _ptrspace);
            ~VM();

            bool run(std::shared_ptr<SysIO> sysIO, const Program &program, uint32_t cycle_budget, std::shared_ptr<Debugger> debugger);
            std::string readString(vmpointer_t p, uint32_t len) {
                return getString(p, len);
            }

            void Jump(uint32_t pos) {
                pc = pos;
            }

            void addInterupt(uint32_t signal, std::function<void(VM*)> interupt) {
                interupts[signal] = interupt;
            }
    };
};

#endif //__EMULATOR_VM_H__
