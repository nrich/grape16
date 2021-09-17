#include "Client/State.h"
#include "Client/DebugState.h"
#include "Common/Keys.h"

using namespace Client;

class Debugger : public Emulator::Debugger {
    public:
        Emulator::OpCode opcode;
        uint32_t pc;
        uint8_t sp;
        uint32_t callstack;
        Emulator::value_t a;
        Emulator::value_t b;
        Emulator::value_t c;
        Emulator::vmpointer_t idx;
        Emulator::value_t memidx;
        uint32_t heap;
        size_t stack;

        void debug(Emulator::OpCode opcode, uint32_t pc, uint8_t sp, uint32_t callstack, Emulator::value_t a, Emulator::value_t b, Emulator::value_t c, Emulator::vmpointer_t idx, Emulator::value_t memidx, uint32_t heap, size_t stack) {
            this->opcode = opcode;
            this->pc = pc;
            this->sp = sp;
            this->callstack = callstack;
            this->a = a;
            this->b = b;
            this->c = c;
            this->idx = idx;
            this->memidx = memidx;
            this->heap = heap;
            this->stack = stack;
        }
};

static std::shared_ptr<Debugger> debugger = std::make_shared<Debugger>();

void DebugState::onRender(State *state, const uint32_t time) {
    auto renderer = state->getRenderer();

    renderer->drawString(0, 0, 8, 8, std::string("OP: ") + Emulator::OpCodeAsString(debugger->opcode)); 
    renderer->drawString(0, 8, 8, 8, std::string("PC: ") + std::to_string(debugger->pc)); 
    renderer->drawString(0, 16, 8, 8, std::string("SP: ") + std::to_string(debugger->sp)); 
    renderer->drawString(0, 24, 8, 8, std::string("CS: ") + std::to_string(debugger->callstack)); 

    renderer->drawString(120, 0, 8, 8, std::string("A: ") + Emulator::ValueToString(debugger->a)); 
    renderer->drawString(120, 8, 8, 8, std::string("B: ") + Emulator::ValueToString(debugger->b)); 
    renderer->drawString(120, 16, 8, 8, std::string("C: ") + Emulator::ValueToString(debugger->c)); 

    renderer->drawString(0, 48, 8, 8, std::string("IDX: ") + std::to_string(debugger->idx)); 
    renderer->drawString(120, 48, 8, 8, std::string("MEM@IDX: ") + Emulator::ValueToString(debugger->memidx)); 

    renderer->drawString(0, 64, 8, 8, std::string("STACK: ") + std::to_string(debugger->stack)); 
    renderer->drawString(0, 72, 8, 8, std::string("HEAP: ") + std::to_string(debugger->heap)); 
}

void DebugState::onTick(State *state, const uint32_t time) {
}

void DebugState::onMouseMove(State *state, const MouseMove &event) {

}

void DebugState::onMouseButtonPress(State *state, const MouseClick &event) {

}

void DebugState::onMouseButtonRelease(State *state, const MouseClick &event) {

}

void DebugState::onKeyDown(State *state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::F2) {
        state->changeState(0);
    } else if (event.keyCode == Common::Keys::Space) {
        vm->run(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), *program, 1, debugger);
    }
}

void DebugState::onKeyUp(State *state, const KeyPress &event) {

}

void DebugState::onEnterState(State *state, std::any data) {
    program = std::any_cast<std::shared_ptr<Emulator::Program>>(data);
}
