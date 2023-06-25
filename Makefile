# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

# Source code and output
SRC_FILE := main.cpp
MODULES := $(filter-out main.cpp, $(wildcard *.cpp))

# Set up compile command
ifeq ($(OS),Windows_NT)
	OS := win
	SRC_DIR := src
	LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	LIB_DIR := $(SRC_DIR)/$(OS)/lib
	INCLUDE_DIR := $(SRC_DIR)/$(OS)
	OUTPUT_DIR := bin/$(OS)
	OUTPUT := $(OUTPUT_DIR)/main.exe

	COMPILE_LINE := $(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(MODULES) $(SRC_FILE) -o $(OUTPUT) $(LIBS)
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S), Darwin)
		OS := mac
		FRAMEWORKS := -F./src/$(OS) -framework SDL2 -framework SDL2_ttf -rpath ./src/$(OS)
		INCLUDE := -I./src/$(OS)/SDL2.framework/Headers -I./src/$(OS)/SDL2_ttf.framework/Headers
		OUTPUT_DIR := bin/$(OS)
		OUTPUT := $(OUTPUT_DIR)/main

		COMPILE_LINE := $(CXX) $(CXXFLAGS) $(INCLUDE) $(FRAMEWORKS) $(MODULES) $(SRC_FILE) -o $(OUTPUT) $(LIBS)
	endif
endif

all: $(OUTPUT)

$(OUTPUT): $(SRC_FILE) $(MODULES)
	$(COMPILE_LINE)

clean:
	rm -rf $(BIN_DIR)
