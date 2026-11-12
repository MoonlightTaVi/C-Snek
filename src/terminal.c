#include <stdio.h>
#include "snek/terminal.h"


/** Erases the screen and resets cursor position. */
#define ANSI_CLEAR "\033[2J\033[H"
/** Text color red. */
#define C_RED "\033[31m"
/** Text color green. */
#define C_GREEN "\033[32m"
/** Reset text color. */
#define C_DEF "\033[0m"


/** @return A string representation of ANSI code for this enum. */
const char* getColor(enum Color c) {
    switch (c) {
        case RED: return C_RED;
        case GREEN: return C_GREEN;
        default: return C_DEF;
    }
}


void clearScreen() {
    printf(ANSI_CLEAR);
}

void printAt(int x, int y, const char* c, enum Color color) {
    printf(
        "%s\033[%d;%dH%s%s",
        getColor(color),
        y, x,
        c,
        getColor(DEFAULT)
    );
}
