## Simple Makefile for SDL2 Checkers game (no Qt)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS   := $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_mixer -lGL -lGLU

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
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SDL_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)


