.PHONY: all clean

### set for Mingw under Cygwin
CXX:=i686-w64-mingw32-g++

### path to SDL
SDL:=/c/SDL

### path to Tcl
TCL:=/c/Tcl
TCLLIB:=$(TCL)/lib/tcl86.lib

### SDL main and mixer libs
LIBS+=$(SDL)/lib/libSDL.dll.a
LIBS+=$(SDL)/lib/SDL_mixer.lib

### should be ok from here
INC+=-I$(TCL)/include
INC+=-I$(SDL)/include
CXXFLAGS+=-Wall $(INC)

all: sdlmix.dll

sdlmix.dll: mixdll.o
	@$(CXX) -shared $(LDFLAGS) -o$@ $^ $(TCLLIB) $(LIBS)

%.o:%.cpp
	@$(CXX) $(CXXFLAGS) -o$@ -c $^

clean:
	@rm -f sdlmix.dll mixdll.o


