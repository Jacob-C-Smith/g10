# Makefile for data project (build artifacts in build/)

# Detect OS for platform-specific settings
UNAME_S := $(shell uname -s)
RPATH_GSDK_REL = ../gsdk/build/lib
ifeq ($(UNAME_S),Darwin)
	SHARED_EXT = dylib
	RPATH_FLAGS = -Wl,-rpath,@loader_path -Wl,-rpath,@loader_path/$(RPATH_GSDK_REL)
else
	SHARED_EXT = so
	RPATH_FLAGS = -Wl,-rpath,'$$ORIGIN' -Wl,-rpath,'$$ORIGIN/$(RPATH_GSDK_REL)'
endif

# SDL3
SDL_CFLAGS := $(shell pkg-config --cflags sdl3 sdl3-image)
SDL_LIBS := $(shell pkg-config --libs sdl3 sdl3-image)

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -Iinclude -Igsdk/include -Igsdk/include/core -Igsdk/include/data -Igsdk/include/performance -Igsdk/include/reflection -std=c23 -g $(SDL_CFLAGS)

# Directories
BUILD_DIR = build
GSDK_LIB_DIR = gsdk/build/lib
G10_SRC_DIR = src

# Sources / objects
G10_SRC = $(wildcard $(G10_SRC_DIR)/*.c)
G10_OBJ = $(patsubst $(G10_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(G10_SRC))

# Library / executables (in build/)
G10_LIB_BASENAME = g10
G10_LIB = $(BUILD_DIR)/lib$(G10_LIB_BASENAME).$(SHARED_EXT)
CLIENT = $(BUILD_DIR)/g10_client

# Locate gsdk shared libraries (full paths)
GSDK_LIBS = $(wildcard $(GSDK_LIB_DIR)/*.$(SHARED_EXT))

# Default target
all: $(G10_LIB) $(SERVER) $(CLIENT)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Object files
$(BUILD_DIR)/%.o: $(G10_SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

# Shared library
$(G10_LIB): $(G10_OBJ)
	$(CC) -shared -o $@ $^ $(GSDK_LIBS) $(SDL_LIBS) 

# Executables
$(CLIENT): main.c $(G10_LIB)
	$(CC) $(CFLAGS) -o $@ $< $(G10_LIB) $(GSDK_LIBS) $(SDL_LIBS) $(RPATH_FLAGS)

# Assets
assets: 

	# shaders
	./scripts/compile-metal-shader.sh quad 
	./scripts/compile-metal-shader.sh geom
	./scripts/compile-metal-shader.sh color 
	./scripts/compile-metal-shader.sh aabb 
	./scripts/compile-metal-shader.sh floor
	./scripts/compile-metal-shader.sh uv
	./scripts/compile-metal-shader.sh texture
	./scripts/compile-metal-shader.sh normal
	./scripts/compile-metal-shader.sh tbn
	./scripts/compile-metal-shader.sh lgt

	# pack geometry
	./scripts/pack-geometry.sh circle
	./scripts/pack-geometry.sh cube
	./scripts/pack-geometry.sh quad

# Info
info:
	@echo "g10 sources : $(G10_SRC)"
	@echo "g10 objects : $(G10_OBJ)"
	@echo "g10 library : $(G10_LIB)"
	@echo "gsdk libraries       : $(GSDK_LIBS)"
	@echo "exec                 : $(CLIENT)"

# Clean
clean:
	rm -rf $(BUILD_DIR)

gport:
	rm gport.zip
	zip gport.zip gport/*
	rm -rf /Users/j/Library/Application\ Support/Blender/3.6/scripts/addons/gport

.PHONY: all clean info assets gport 
