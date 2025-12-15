## Simple Makefile for Raylib Checkers game

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

# Try to use pkg-config for Raylib, otherwise use defaults
RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null || echo "-I/usr/local/include -I/usr/include")
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null || echo "-lraylib -lm -lpthread -ldl -lrt -lX11")

SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
BIN_DIR   := bin

SRC := $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/GameLogic.cpp \
       $(SRC_DIR)/CheckersAI.cpp \
       $(SRC_DIR)/SoundManager.cpp \
       $(SRC_DIR)/Renderer.cpp

OBJ := $(BUILD_DIR)/main.o \
       $(BUILD_DIR)/GameLogic.o \
       $(BUILD_DIR)/CheckersAI.o \
       $(BUILD_DIR)/SoundManager.o \
       $(BUILD_DIR)/Renderer.o

TARGET := $(BIN_DIR)/checkers_sdl

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(RAYLIB_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(RAYLIB_CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)


