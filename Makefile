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
G10_SRC = $(wildcard $(G10_SRC_DIR)/*.c) $(wildcard $(G10_SRC_DIR)/*/*.c)
G10_OBJ = $(patsubst $(G10_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(G10_SRC))

# Library / executables (in build/)
G10_LIB_BASENAME = g10
G10_LIB = $(BUILD_DIR)/lib$(G10_LIB_BASENAME).$(SHARED_EXT)
CLIENT = $(BUILD_DIR)/g10_client
LIGHTSPEED = $(BUILD_DIR)/lightspeed

# Locate gsdk shared libraries (full paths)
GSDK_LIBS = $(wildcard $(GSDK_LIB_DIR)/*.$(SHARED_EXT))

# Default target
all: $(G10_LIB) $(CLIENT) $(LIGHTSPEED) transform_info

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Object files
$(BUILD_DIR)/%.o: $(G10_SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

# Shared library
$(G10_LIB): $(G10_OBJ)
	$(CC) -shared -o $@ $^ $(GSDK_LIBS) $(SDL_LIBS) 

# Executables
$(CLIENT): main.c $(G10_LIB)
	$(CC) $(CFLAGS) -o $@ $< $(G10_LIB) $(GSDK_LIBS) $(SDL_LIBS) $(RPATH_FLAGS)

$(LIGHTSPEED): lightspeed/main.c $(G10_LIB)
	$(CC) $(CFLAGS) -o $@ $< $(G10_LIB) $(GSDK_LIBS) $(SDL_LIBS) $(RPATH_FLAGS)

transform_info: util/transform/info.c $(G10_LIB)
	$(CC) $(CFLAGS) -o $@ $< $(G10_LIB) $(GSDK_LIBS) $(SDL_LIBS) $(RPATH_FLAGS)

# Assets
assets: 

	# ensure geometry directory exists
	mkdir -p ./assets/input/entity
	mkdir -p ./assets/input/geometry
	mkdir -p ./assets/input/material
	mkdir -p ./assets/input/scene
	mkdir -p ./assets/input/texture

	rm -rf ./assets/entity/*
	rm -rf ./assets/geometry/*
	rm -rf ./assets/material/*
	rm -rf ./assets/scene/*

	# geometry
	@./scripts/geometry/gport-geometry.sh g10_test_room #| grep 'gport'
	@./scripts/geometry/gport-geometry.sh lightspeed #| grep 'gport'
	@./scripts/geometry/gport-geometry.sh g10_base_geometry #| grep 'gport'

	# texture
	@./scripts/texture/gport-texture.sh g10_base_geometry #| grep 'gport'
	@./scripts/texture/gport-texture.sh g10_test_room #| grep 'gport'
	@./scripts/texture/gport-texture.sh lightspeed #| grep 'gport'

	# material
	@./scripts/material/gport-material.sh g10_test_room #| grep 'gport'
	@./scripts/material/gport-material.sh lightspeed #| grep 'gport'
	
	# entities
	@./scripts/entity/gport-entity.sh g10_base_geometry #| grep 'gport'
	@./scripts/entity/gport-entity.sh g10_test_room #| grep 'gport'

	# scene
	@./scripts/scene/gport-scene.sh g10_test_room #| grep 'gport'
	@./scripts/scene/gport-scene.sh lightspeed #| grep 'gport'

	# shaders
	@./scripts/pipeline/compile-metal-shader.sh quad 
	@./scripts/pipeline/compile-metal-shader.sh geom
	@./scripts/pipeline/compile-metal-shader.sh color 
	@./scripts/pipeline/compile-metal-shader.sh aabb 
	@./scripts/pipeline/compile-metal-shader.sh floor
	@./scripts/pipeline/compile-metal-shader.sh uv
	@./scripts/pipeline/compile-metal-shader.sh texture
	@./scripts/pipeline/compile-metal-shader.sh normal
	@./scripts/pipeline/compile-metal-shader.sh tbn
	@./scripts/pipeline/compile-metal-shader.sh skybox
	@./scripts/pipeline/compile-metal-shader.sh default

	mv ./assets/input/entity/* ./assets/entity/
	mv ./assets/input/geometry/* ./assets/geometry/
	mv ./assets/input/material/* ./assets/material/
	mv ./assets/input/texture/* ./assets/texture/
	mv ./assets/input/scene/* ./assets/scene/

	rm -rf ./assets/input/entity || exit 0
	rm -rf ./assets/input/geometry || exit 0
	rm -rf ./assets/input/material || exit 0
	rm -rf ./assets/input/texture || exit 0
	rm -rf ./assets/input/scene || exit 0
	
	touch ./assets/input/ignore

# Assets 2
assets2:
	@./scripts/assets/gport-assets.sh assets

# Info
info:
	@echo "g10 sources    : $(G10_SRC)"
	@echo "g10 objects    : $(G10_OBJ)"
	@echo "g10 library    : $(G10_LIB)"
	@echo "gsdk libraries : $(GSDK_LIBS)"
	@echo "exec           : $(CLIENT)"
	@echo "exec           : $(LIGHTSPEED)"

# Clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf assets/gport_cache.json

gport:
	rm gport.zip
	zip gport.zip gport/*
	rm -rf /Users/j/Library/Application\ Support/Blender/3.6/scripts/addons/gport
	unzip gport.zip -d /Users/j/Library/Application\ Support/Blender/3.6/scripts/addons

.PHONY: all clean info assets assets2 gport 
