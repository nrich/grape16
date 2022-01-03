#include "Client/State.h"
#include "Client/DebugState.h"
#include "Common/Keys.h"

#include "Emulator/VM.h"

#include <stack>
#include <vector>

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
        uint32_t heapidx;
        std::stack<Emulator::value_t> stack;
        std::vector<Emulator::value_t> mem;
        std::vector<Emulator::value_t> heap;

        void debug(Emulator::OpCode opcode, uint32_t pc, uint8_t sp, uint32_t callstack, Emulator::value_t a, Emulator::value_t b, Emulator::value_t c, Emulator::vmpointer_t idx, Emulator::value_t memidx, uint32_t heapidx, std::stack<Emulator::value_t> stack, std::vector<Emulator::value_t> mem, std::vector<Emulator::value_t> heap) {
            this->opcode = opcode;
            this->pc = pc;
            this->sp = sp;
            this->callstack = callstack;
            this->a = a;
            this->b = b;
            this->c = c;
            this->idx = idx;
            this->memidx = memidx;
            this->heapidx = heapidx;
            this->stack = stack;

            this->mem = mem;
            this->heap = heap;
        }
};

static std::shared_ptr<Debugger> debugger = std::make_shared<Debugger>();

void DebugState::onRender(State *state, const uint32_t time) {
    auto renderer = state->getRenderer();

    renderer->drawString(0, 0, 8, 8, std::string("OP: ") + Emulator::OpCodeAsString(debugger->opcode)); 
    renderer->drawString(0, 8, 8, 8, std::string("PC: ") + std::to_string(debugger->pc)); 
    renderer->drawString(0, 16, 8, 8, std::string("SP: ") + std::to_string(debugger->sp)); 
    renderer->drawString(0, 24, 8, 8, std::string("CS: ") + std::to_string(debugger->callstack)); 

    renderer->drawString(144, 0, 8, 8, std::string("A: ") + Emulator::ValueToString(debugger->a));
    renderer->drawString(144, 8, 8, 8, std::string("B: ") + Emulator::ValueToString(debugger->b));
    renderer->drawString(144, 16, 8, 8, std::string("C: ") + Emulator::ValueToString(debugger->c));

    std::stringstream ss;
    ss << std::hex << debugger->idx;

    renderer->drawString(0, 48, 8, 8, std::string("IDX: &") + ss.str()); 
    renderer->drawString(144, 48, 8, 8, std::string("MEM@IDX: ") + Emulator::ValueToString(debugger->memidx));


    if (debugger->stack.size() > 0) {
        renderer->drawString(0, 64, 8, 8, std::string("STACK: ") + Emulator::ValueToString(debugger->stack.top()));
    } else {
        renderer->drawString(0, 64, 8, 8, std::string("STACK: <EMPTY>"));
    }

    renderer->drawString(144, 64, 8, 8, std::string("SIZE: ") + std::to_string(debugger->stack.size()));

    std::stringstream stream;
    stream << std::hex << debugger->heapidx;

    renderer->drawString(0, 72, 8, 8, std::string("HEAP: &") + stream.str());

    renderer->drawString(0, 88, 8, 8, std::string("EXCEPTION: ") + exception);

    size_t xoffset = 0;
    size_t yoffset = 0;

    for (size_t i = 0; i < debugger->mem.size(); i++) {
        xoffset = i / 16;
        yoffset = i % 16;

        renderer->drawString(0 + xoffset*64, 104+(yoffset*8), 8, 8, std::to_string(i));
        renderer->drawString(24 + xoffset*64, 104+(yoffset*8), 8, 8, std::string(Emulator::ValueToString(debugger->mem[i])));
    }

    for (size_t i = 0; i < debugger->heap.size(); i++) {
        xoffset = (i + debugger->mem.size()) / 16;
        yoffset = (i + debugger->mem.size()) % 16;

        renderer->drawString(0 + xoffset*64, 104+(yoffset*8), 8, 8, std::to_string(i));
        renderer->drawString(24 + xoffset*64, 104+(yoffset*8), 8, 8, std::string(Emulator::ValueToString(debugger->heap[i])));
    }

}

void DebugState::onTick(State *state, const uint32_t time) {
    sysio->setTime(time);
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
        try {
            vm->run(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), *program, 1, debugger);
            exception = "<NONE>";
        } catch (const std::runtime_error &re) {
            exception = std::string(re.what());
        }
    } else if (event.keyCode == Common::Keys::R) {
        try {
            bool done = false;
            while (!done) {
                done = vm->run(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), *program, clockspeed, debugger);
            }
        } catch (const std::runtime_error &re) {
            //sysio->puts(std::string("Runtime Error: ") + re.what() + std::string("\n"));
            std::cerr << "Runtime Error: " << re.what() << std::endl;
        }
    }
}

void DebugState::onKeyUp(State *state, const KeyPress &event) {

}

void DebugState::onEnterState(State *state, std::any data) {
    program = std::any_cast<std::shared_ptr<Emulator::Program>>(data);
}
