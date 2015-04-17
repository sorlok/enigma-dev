SOURCES += $(wildcard Graphics_Systems/OpenGL1/*.cpp) $(wildcard Graphics_Systems/General/*.cpp) Graphics_Systems/General/glew.c

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL
endif

