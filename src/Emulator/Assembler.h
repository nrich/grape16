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
        SHORT,
        FLOAT,
        POINTER,
        STRING,
        VALUE,
        SYSCALL,
        COUNT
    };

    struct AsmToken {
        OpCode opcode;
        std::optional<std::variant<int16_t, float, uint32_t, std::string>> arg;

        AsmToken(OpCode opcode) : opcode(opcode) {
            arg = std::nullopt;
        }

        AsmToken(OpCode opcode, int16_t i) : opcode(opcode), arg(i) {
        }

        AsmToken(OpCode opcode, float f) : opcode(opcode), arg(f) {
        }

        AsmToken(OpCode opcode, uint32_t vp) : opcode(opcode), arg(vp) {
        }

        AsmToken(OpCode opcode, const std::string &str) : opcode(opcode), arg(str) {
        }

    };
};

Emulator::AsmToken parseAsmLine(const std::string &line);
std::vector<Emulator::AsmToken> parseAsmFile(const std::string &filename);

void assemble(std::vector<Emulator::AsmToken> lines, Emulator::Program &program);

#endif //__EMULATOR_ASSEMBLER_H__
