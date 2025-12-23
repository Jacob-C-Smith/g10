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
SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL_LIBS := $(shell pkg-config --libs sdl3)

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
	xcrun metal -c assets/pipeline/quad/vert.metal -o assets/pipeline/quad/vert.air
	xcrun metal -c assets/pipeline/quad/frag.metal -o assets/pipeline/quad/frag.air
	xcrun metallib assets/pipeline/quad/vert.air assets/pipeline/quad/frag.air -o assets/pipeline/quad/quad.metallib

	xcrun metal -c assets/pipeline/geom/vert.metal -o assets/pipeline/geom/vert.air
	xcrun metal -c assets/pipeline/geom/frag.metal -o assets/pipeline/geom/frag.air
	xcrun metallib assets/pipeline/geom/vert.air assets/pipeline/geom/frag.air -o assets/pipeline/geom/geom.metallib

	xcrun metal -c assets/pipeline/color/vert.metal -o assets/pipeline/color/vert.air
	xcrun metal -c assets/pipeline/color/frag.metal -o assets/pipeline/color/frag.air
	xcrun metallib assets/pipeline/color/vert.air assets/pipeline/color/frag.air -o assets/pipeline/color/color.metallib

	xcrun metal -c assets/pipeline/aabb/vert.metal -o assets/pipeline/aabb/vert.air
	xcrun metal -c assets/pipeline/aabb/frag.metal -o assets/pipeline/aabb/frag.air
	xcrun metallib assets/pipeline/aabb/vert.air assets/pipeline/aabb/frag.air -o assets/pipeline/aabb/aabb.metallib

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

gport_symlink:
	rm -rf /Users/j/Library/Application\ Support/Blender/3.6/scripts/addons/gport
	ln -s ./gport/ /Users/j/Library/Application\ Support/Blender/3.6/scripts/addons/gport

.PHONY: all clean info assets gport gport_symlink
