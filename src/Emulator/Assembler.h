#ifndef __EMULATOR_ASSEMBLER_H__
#define __EMULATOR_ASSEMBLER_H__

#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <iostream>

#include <optional>
#include <variant>

#include "Emulator/VM.h"

namespace Emulator {
    enum class ArgType {
        NONE,
        BYTE,
        SHORT,
        FLOAT,
        POINTER,
        STRING,
        VALUE,
        SYSCALL,
        COUNT
    };

    struct AsmToken {
        OpCode type;
        std::optional<std::variant<uint8_t, int16_t, float, uint32_t>> arg;

        AsmToken(OpCode type, uint8_t b) : type(type), arg(b) {
        }

        AsmToken(OpCode type, int16_t i) : type(type), arg(i) {
        }

        AsmToken(OpCode type, float f) : type(type), arg(f) {
        }

        AsmToken(OpCode type, uint32_t vp) : type(type), arg(vp) {
        }

    };
};

Emulator::AsmToken parseAsmLine(const std::string &line);
std::vector<Emulator::AsmToken> parseAsmFile(const std::string &filename);

#endif //__EMULATOR_ASSEMBLER_H__
