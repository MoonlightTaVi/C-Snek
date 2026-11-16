# Run from the root directory

CC=gcc
CFLAGS=-c -Wall -Wextra -Werror

APPNAME=Snek

SRC_DIR=./src
BUILD_DIR=./build


DEPS = -I./include/
OBJS = \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/terminal.o \
	$(BUILD_DIR)/core.o


all: clean build run

build: main.o terminal.o core.o
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

core.o:
	$(CC) $(CFLAGS) $(DEPS) $(SRC_DIR)/core.c -o $(BUILD_DIR)/core.o
