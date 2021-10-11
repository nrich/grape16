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
        LABEL,
        COUNT
    };

    struct AsmToken {
        OpCode opcode;
        std::optional<std::variant<int16_t, float, int32_t, value_t, std::string, std::pair<SysCall, RuntimeValue>>> arg;

        AsmToken(OpCode opcode) : opcode(opcode) {
            arg = std::nullopt;
        }

        AsmToken(OpCode opcode, int16_t i) : opcode(opcode), arg(i) {
        }

        AsmToken(OpCode opcode, float f) : opcode(opcode), arg(f) {
        }

        AsmToken(OpCode opcode, int32_t p) : opcode(opcode), arg(p) {
        }

        AsmToken(OpCode opcode, value_t v) : opcode(opcode), arg(v) {
        }

        AsmToken(OpCode opcode, const std::string &str) : opcode(opcode), arg(str) {
        }

        AsmToken(OpCode opcode, std::pair<SysCall, RuntimeValue> syscall) : opcode(opcode), arg(syscall) {
        }

        size_t size() const {
            if (arg == std::nullopt) {
                if (std::holds_alternative<int16_t>(*arg)) {
                    return 1+2;
                } else if (std::holds_alternative<float>(*arg)) {
                    return 1+4;
                } else if (std::holds_alternative<int32_t>(*arg)) {
                    return 1+3;
                } else if (std::holds_alternative<value_t>(*arg)) {
                    return 1+4;
                } else if (std::holds_alternative<std::string>(*arg)) {
                    auto value = std::get<std::string>(*arg);
                    return 1 + value.size();
                } else if (std::holds_alternative<std::pair<SysCall, RuntimeValue>>(*arg)) {
                    return 1+2+2;
                } else {
                    std::cerr << "Error in size " << (int)opcode << std::endl;
                }
            }
            return 1;
        }
    };
};

std::vector<Emulator::AsmToken> parseAsmFile(const std::string &filename);

void assemble(std::vector<Emulator::AsmToken> lines, Emulator::Program &program);
std::vector<Emulator::AsmToken> disassemble(const Emulator::Program &program);
std::string AsmTokenAsString(const Emulator::AsmToken &token);
Emulator::Program optimize(const Emulator::Program &code);
std::string ProgramAsString(const Emulator::Program &program, bool resolve_jumps=false);

#endif //__EMULATOR_ASSEMBLER_H__
