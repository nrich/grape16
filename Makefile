MKDIR = mkdir -p
CXX = g++
RM = rm -f
RMDIR = rm -rf
INC = -I src
LDFLAGS = $(shell sdl2-config --libs) -lGL -lGLU -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lglfw -lstdc++ -lncurses -lportaudio
CPPFLAGS = -g -std=c++17 $(INC) -Wall $(shell sdl2-config --cflags)
STRIP = strip
 
ifdef CONFIG_W32
    CXX = i686-w64-mingw32-g++
    ifdef CONFIG_MIN
        LDFLAGS = -static-libgcc -static-libstdc++ -L/opt/local/glfw-3.3.4.bin.WIN32/lib-mingw-w64/ -L/opt/local/portaudio-i686-w64/lib/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lglfw3 -lportaudio -lwinmm -lgdi32 -lsetupapi -lole32 -loleaut32 
        INC = -I src -I /opt/local/glfw-3.3.4.bin.WIN32/include -I /opt/local/mingw-std-threads-1.0.0 -I /opt/local/portaudio-i686-w64/include 
        CPPFLAGS = -O3 -std=c++17  $(INC) -D_WIN32 -Wall
    else
        LDFLAGS = -mconsole -static-libgcc -static-libstdc++ $(shell /opt/local/i686-w64-mingw32/bin/sdl2-config --libs --static-libs) -L/opt/local/glfw-3.3.4.bin.WIN32/lib-mingw-w64/ -L/opt/local/portaudio-i686-w64/lib/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lglfw3 -lgdi32 -lportaudio -lwinmm -lgdi32 -lsetupapi -lole32 -loleaut32  -mconsole
        INC = -I src -I /opt/local/glfw-3.3.4.bin.WIN32/include -I /opt/local/mingw-std-threads-1.0.0 -I /opt/local/portaudio-i686-w64/include 
        CPPFLAGS = -O3 -std=c++17  -pthread $(INC) -D_WIN32 -Wall $(shell /opt/local/i686-w64-mingw32/bin/sdl2-config --cflags) 
    endif
    WINDRES = i686-w64-mingw32-windres
    STRIP = i686-w64-mingw32-strip
endif
 
ifdef CONFIG_W64
    CXX = x86_64-w64-mingw32-g++
    ifdef CONFIG_MIN
        LDFLAGS = -static-libgcc -static-libstdc++ -L/opt/local/glfw-3.3.4.bin.WIN64/lib-mingw-w64/ -L/opt/local/portaudio-x86_64-w64/lib/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lglfw3 -lportaudio -lwinmm -lgdi32 -lsetupapi -lole32 -loleaut32
        INC = -I src -I /opt/local/glfw-3.3.4.bin.WIN64/include -I /opt/local/mingw-std-threads-1.0.0 -I /opt/local/portaudio-x86_64-w64/include
        CPPFLAGS = -O3 -std=c++17  $(INC) -D_WIN32 -Wall
    else
        LDFLAGS = -static-libgcc -static-libstdc++ $(shell /opt/local/x86_64-w64-mingw32/bin/sdl2-config --libs --static-libs) -L /opt/local/glfw-3.3.4.bin.WIN64/lib-mingw-w64/ -L/opt/local/portaudio-x86_64-w64/lib/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lglfw3 -lgdi32 -lportaudio -lwinmm -lgdi32 -lsetupapi -lole32 -loleaut32 -mconsole
        INC = -I src -I /opt/local/glfw-3.3.4.bin.WIN64/include -I /opt/local/mingw-std-threads-1.0.0 -I /opt/local/portaudio-x86_64-w64/include 
        CPPFLAGS = -O3 -std=c++17  -pthread $(INC) -pthread -D_WIN64 -Wall $(shell /opt/local/x86_64-w64-mingw32/bin/sdl2-config --cflags)
    endif
    WINDRES = x86_64-w64-mingw32-windres
    STRIP = x86_64-w64-mingw32-strip
endif

ifdef CONFIG_JS
    CXX = em++
    INC = -I /opt/local/gl4es-emcc/include -I src
    LDFLAGS = $(shell $(EMSDK)/upstream/emscripten/system/bin/sdl2-config --libs) -L/opt/local/gl4es-emcc/lib -lGL -lGLU -fexceptions -s EXIT_RUNTIME=0 -s VERBOSE=0 -s FORCE_ALIGNED_MEMORY=0 -s ALLOW_MEMORY_GROWTH=1 -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s USE_SDL=2 -s LEGACY_GL_EMULATION=0 -s FULL_ES2=1 -s FULL_ES3=1 -s WASM=1 -s ASYNCIFY=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 /opt/local/gl4es-emcc/lib/libGL.a
    CPPFLAGS = -O3 -std=c++17 $(INC) -Wall $(shell $(EMSDK)/upstream/emscripten/system/bin/sdl2-config --cflags) -fexceptions
endif

VERSION = $(shell cat VERSION.txt)
CPPFLAGS := $(CPPFLAGS) -DVERSION="\"$(VERSION)\""
ifdef CONFIG_MIN
CPPFLAGS := $(CPPFLAGS) -DMINBUILD="1"
endif
LDFLAGS := $(LDFLAGS)
 
# Temporary build directories
ifdef CONFIG_W32
    BUILD := .win32
else ifdef CONFIG_W64
    BUILD := .win64
else ifdef CONFIG_JS
    BUILD := .emcc
else
    BUILD := .nix
endif
 
# Define V=1 to show command line.
ifdef V
    Q :=
    E := @true
else
    Q := @
    E := @echo
endif
 
ifdef CONFIG_W32
    TARG := grape16.exe
else ifdef CONFIG_W64
    TARG := grape16x64.exe
else ifdef CONFIG_JS
    TARG := grape16.html
else
    TARG := grape16
endif
 
all: $(TARG)
 
default: all
 
.PHONY: all default clean strip
 
COMMON_OBJS := \
	src/Audio/Tone.o \
	src/Client/BaseState.o \
	src/Client/DebugState.o \
	src/Client/DisplayMenuState.o \
	src/Client/EmulatorState.o \
	src/Client/LoadingState.o \
	src/Client/State.o \
	src/Common/Colour.o \
	src/Common/DisplayMode.o \
        src/Emulator/VM.o \
        src/Emulator/Assembler.o \
        src/Emulator/Basic.o \
	src/Renderer/Base.o \
	src/main.o 

ifdef CONFIG_JS
COMMON_OBJS := \
	$(COMMON_OBJS) \
	src/Renderer/Immediate.o \
	src/Renderer/Immediate/Font.o
else
COMMON_OBJS := \
	$(COMMON_OBJS) \
	src/Renderer/Immediate.o \
	src/Renderer/Immediate/Font.o
endif

ifndef CONFIG_MIN
COMMON_OBJS := \
	$(COMMON_OBJS) \
	src/Sys/SDL2.o
endif

ifdef CONFIG_W32
OBJS := \
	$(COMMON_OBJS) \
	src/Sys/GLFW.o \
	grape16.res
else ifdef CONFIG_W64
OBJS := \
	$(COMMON_OBJS) \
	src/Sys/GLFW.o \
	grape16.res
else ifdef CONFIG_JS
OBJS := \
	$(COMMON_OBJS) 
else
OBJS := \
	$(COMMON_OBJS) \
        src/Renderer/NCurses.o \
        src/Sys/NCurses.o \
	src/Sys/GLFW.o \
	src/Sys/SFML.o
endif
 
# Rewrite paths to build directories
OBJS := $(patsubst %,$(BUILD)/%,$(OBJS))

$(TARG): $(OBJS)
	$(E) [LD] $@    
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(E) [CLEAN]
	$(Q)$(RM) $(TARG)
	$(Q)$(RMDIR) $(BUILD)

strip: $(TARG)
	$(E) [STRIP]
	$(Q)$(STRIP) $(TARG)

$(BUILD)/%.o: %.cpp
	$(E) [CXX] $@
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CXX) -c $(CPPFLAGS) -o $@ $<

$(BUILD)/%.res: %.rc
	$(E) [RES] $@
	$(Q)$(WINDRES) $< -O coff -o $@
