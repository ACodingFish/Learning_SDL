
INC_DIR = ./inc

CC=gcc
CFLAGS= -g -Wall -I $(INC_DIR)

# Source file directories
SRC_DIR=./src
SDL_LIB=$(SRC_DIR)/SDL_Lib

SRC=$(SRC_DIR)/main.c \
	$(SDL_LIB)/SDL_Manager.c \
	$(SDL_LIB)/Screen.c \
	$(SDL_LIB)/Screen_Draw.c \
	$(SDL_LIB)/GFX_Manager.c \
	$(SDL_LIB)/Screen_Scale.c \

# Include Files (Dependencies)
DEPS=$(INC_DIR)/*.h

# Executable Name
TARGET=Screen

# Flags needed for SDL compilation
SDL_FLAGS= `sdl2-config --cflags --libs`

all: $(TARGET)

$(TARGET): $(DEPS) $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(SDL_FLAGS)

# @ to not echo
clean:
	@rm $(TARGET)
