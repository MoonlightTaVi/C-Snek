# Run from the root directory

CC=gcc
CFLAGS=-c -Wall -Wextra -Werror

APPNAME=Snek

SRC_DIR=./src
BUILD_DIR=./build


DEPS = -I./include/
OBJS = $(BUILD_DIR)/main.o \
	$(BUILD_DIR)/terminal.o


all: clean build run

build: main.o terminal.o
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(APPNAME)
	chmod +x $(BUILD_DIR)/$(APPNAME)

clean:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

run:
	./$(BUILD_DIR)/$(APPNAME)


main.o:
	$(CC) $(CFLAGS) $(DEPS) $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

terminal.o:
	$(CC) $(CFLAGS) $(DEPS) $(SRC_DIR)/terminal.c -o $(BUILD_DIR)/terminal.o
