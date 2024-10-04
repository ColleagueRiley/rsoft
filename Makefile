# CUSTOM ARGS :
# RGFW_WAYLAND=1 -> use wayland 
# NO_VULKAN=1 -> do not compile the vulkan example
# NO_GLES=1 -> do not compile the gles example (on by default for non-linux OSes)

CC = gcc
AR = ar

# used for compiling RGFW.o
CUSTOM_CFLAGS =
# used for the examples
CFLAGS = 

LIBS := -static -lgdi32 -lm -lwinmm -ggdb
EXT = .exe
LIB_EXT = .dll

LIBS += -D _WIN32_WINNT="0x0501"

WARNIGNS = 
OS_DIR = \\

detected_OS = windows

OBJ_FILE = .o

# not using a cross compiler
ifeq (,$(filter $(CC),x86_64-w64-mingw32-gcc i686-w64-mingw32-gcc x86_64-w64-mingw32-g++ /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe))
	detected_OS := $(shell uname 2>/dev/null || echo Unknown)

	ifeq ($(detected_OS),Darwin)        # Mac OS X
		LIBS := -lm -framework Foundation -framework AppKit -framework CoreVideo
		VULKAN_LIBS = -lm  -framework Foundation -framework AppKit --framework CoreVideo -lvulkan
		EXT =
		LIB_EXT = .dylib
		OS_DIR = /
	endif
	ifeq ($(detected_OS),Linux)
    	LIBS := -lXrandr -lX11 -lm -ldl -lpthread
		VULKAN_LIBS = -lX11 -lXrandr -lm -ldl -lpthread -lvulkan
		EXT =
		LIB_EXT = .so
		OS_DIR = /
	endif
else
	OS_DIR = /
endif

LINK_GL1 =
LINK_GL3 =
LINK_GL2 =

ifneq (,$(filter $(CC),cl /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe))
	WARNINGS =
	LIBS = /static
	DX11_LIBS =
	VULKAN_LIBS = 
	OBJ_FILE = .obj
else ifeq ($(CC),emcc)
	LINK_GL1 = -s LEGACY_GL_EMULATION -D LEGACY_GL_EMULATION -sGL_UNSAFE_OPTS=0
	LINK_GL3 = -s FULL_ES3 -s USE_WEBGL2 
	LINK_GL2 = -s FULL_ES2 -s USE_WEBGL2 
	EXPORTED_JS = -s EXPORTED_RUNTIME_METHODS="['stringToNewUTF8']"
	LIBS = -s WASM=1 -s ASYNCIFY -s GL_SUPPORT_EXPLICIT_SWAP_CONTROL=1 $(EXPORTED_JS)
	EXT = .js
	NO_GLES = 0
	NO_VULKAN = 1
	detected_OS = web

	LIBS += --preload-file ./
else
	LIBS += -std=c99 
endif

EXAMPLE_OUTPUTS = \
    examples/shapes \
    examples/textures \
	examples/raycaster \
	examples/doom-like \
	examples/text \

all: $(EXAMPLE_OUTPUTS)

examples: $(EXAMPLE_OUTPUTS)

$(EXAMPLE_OUTPUTS): %: %.c RSoft.h
	$(CC) $(CFLAGS) $(WARNINGS) -I. $< $(LIBS) $(LINK_GL1)  -o $@$(EXT)

debug: all
	@for exe in $(EXAMPLE_OUTPUTS); do \
		echo "Running $$exe..."; \
		.$(OS_DIR)$$exe$(EXT); \
	done

clean:
	rm -f $(EXAMPLE_OUTPUTS)$(OS_DIR)examples$(OS_DIR)*.exe .$(OS_DIR)examples$(OS_DIR)*.js .$(OS_DIR)examples$(OS_DIR)*.wasm 
	

.PHONY: all examples clean

