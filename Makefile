MKDIR = mkdir -p
CXX = g++
RM = rm -f
RMDIR = rm -rf
INC = -I src
LDFLAGS = $(shell sdl2-config --libs) -lGL -lGLU -lsfml-graphics -lsfml-window -lsfml-system -lglfw -lstdc++ -lncurses
CPPFLAGS = -std=c++17 $(INC) -Wall $(shell sdl2-config --cflags)
STRIP = strip
 
ifdef CONFIG_W32
    CXX = i686-w64-mingw32-g++
    ifdef CONFIG_MIN
        LDFLAGS = -L/opt/local/glfw-3.3.4.bin.WIN32/lib-mingw-w64/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lglfw3 -lgdi32 
        INC = -I src -I /opt/local/glfw-3.3.4.bin.WIN32/include -I /opt/local/mingw-std-threads-1.0.0
        CPPFLAGS = -O3 -std=c++17  $(INC) -D_WIN32 -Wall
    else
        LDFLAGS = $(shell /opt/local/i686-w64-mingw32/bin/sdl2-config --libs --static-libs) -L/opt/local/SFML-i686/lib -L/opt/local/glfw-3.3.4.bin.WIN32/lib-mingw-w64/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lsfml-graphics -lsfml-window -lsfml-system -lglfw3 -lgdi32
        INC = -I src -I /opt/local/SFML-i686/include -I /opt/local/glfw-3.3.4.bin.WIN32/include -I /opt/local/mingw-std-threads-1.0.0
        CPPFLAGS = -O3 -std=c++17  -pthread $(INC) -D_WIN32 -Wall $(shell /opt/local/i686-w64-mingw32/bin/sdl2-config --cflags)
    endif
    WINDRES = i686-w64-mingw32-windres
    STRIP = i686-w64-mingw32-strip
endif
 
ifdef CONFIG_W64
    CXX = x86_64-w64-mingw32-g++
    ifdef CONFIG_MIN
        LDFLAGS = -L/opt/local/glfw-3.3.4.bin.WIN64/lib-mingw-w64/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lglfw3 -lgdi32 
        INC = -I src -I /opt/local/glfw-3.3.4.bin.WIN64/include -I /opt/local/mingw-std-threads-1.0.0
        CPPFLAGS = -O3 -std=c++17  $(INC) -D_WIN32 -Wall
    else
        LDFLAGS = $(shell /opt/local/x86_64-w64-mingw32/bin/sdl2-config --libs --static-libs) -L/opt/local/SFML-x86_64/lib -L /opt/local/glfw-3.3.4.bin.WIN64/lib-mingw-w64/ -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -lwsock32 -lwinspool -lws2_32 -lsfml-graphics -lsfml-window -lsfml-system -lglfw3 -lgdi32
        INC = -I src -I/opt/local/SFML-x86_64/include -I /opt/local/glfw-3.3.4.bin.WIN64/include -I /opt/local/mingw-std-threads-1.0.0
        CPPFLAGS = -O3 -std=c++17  -pthread $(INC) -pthread -D_WIN64 -Wall $(shell /opt/local/x86_64-w64-mingw32/bin/sdl2-config --cflags)
    endif
    WINDRES = x86_64-w64-mingw32-windres
    STRIP = x86_64-w64-mingw32-strip
endif


VERSION = $(shell cat VERSION.txt)
CPPFLAGS := $(CPPFLAGS) -DVERSION="\"$(VERSION)\""
LDFLAGS := $(LDFLAGS)
 
# Temporary build directories
ifdef CONFIG_W32
    BUILD := .win32
else ifdef CONFIG_W64
    BUILD := .win64
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
else
    TARG := grape16
endif
 
all: $(TARG)
 
default: all
 
.PHONY: all default clean strip
 
COMMON_OBJS := \
	src/Client/BaseState.o \
	src/Client/DisplayMenuState.o \
	src/Client/EmulatorState.o \
	src/Client/LoadingState.o \
	src/Client/State.o \
	src/Common/Colour.o \
	src/Common/DisplayMode.o \
        src/Emulator/VM.o \
        src/Emulator/Basic.o \
	src/Renderer/Base.o \
	src/Renderer/Immediate.o \
	src/Renderer/Immediate/Font.o \
	src/Sys/GLFW.o \
	src/main.o 

ifndef CONFIG_MIN
COMMON_OBJS := \
	$(COMMON_OBJS) \
	src/Sys/SDL2.o \
	src/Sys/SFML.o
endif
 
ifdef CONFIG_W32
OBJS := \
	$(COMMON_OBJS) \
	#space.res
else ifdef CONFIG_W64
OBJS := \
	$(COMMON_OBJS) \
	#space.res
else
OBJS := \
	$(COMMON_OBJS) \
        src/Renderer/NCurses.o \
        src/Sys/NCurses.o
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
