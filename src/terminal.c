#include "snek/terminal.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>


/** Erases the screen and resets cursor position. */
#define ANSI_CLEAR "\033[2J\033[H"
/** Text color red. */
#define COLOR_RED "\033[31m"
/** Text color green. */
#define COLOR_GREEN "\033[32m"
/** Reset text color. */
#define COLOR_DEF "\033[0m"

/** The default file descriptor for standard input. */
const int g_FD = STDIN_FILENO;
/** The global active TTY state. */
struct termios g_tty_current;
/** The global default TTY state, to reset to after quitting. */
struct termios g_tty_default;
/** The terminal window size, from <sys/ioctl>. */
struct winsize g_winsize; 


/** @return A string representation of ANSI code for this enum. */
const char* getColor(enum Color c) {
    switch (c) {
        case C_RED: return COLOR_RED;
        case C_GREEN: return COLOR_GREEN;
        default: return COLOR_DEF;
    }
}


void clearScreen() {
    printf(ANSI_CLEAR);
}

void printAt(int x, int y, const char* c, enum Color color) {
    printf(
        "\033[%d;%dH%s%s%s",
        y, x,
        getColor(color),
        c,
        getColor(C_DEFAULT)
    );
}

int initializeTerminalSuccess() {
    if (!isatty(g_FD))
        return 0;
    // Read window size (does not update automatically if resized)
    ioctl(g_FD, TIOCGWINSZ, &g_winsize);
    // Disable STDOUT buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    // Read current attributes
    if (tcgetattr(g_FD, &g_tty_current) == -1)
        return 0;    
    if (tcgetattr(g_FD, &g_tty_default) == -1)
        return 0;    

    g_tty_current.c_lflag &= ~ICANON;
    g_tty_current.c_lflag &= ~ECHO;
    g_tty_current.c_cc[VTIME] = 1;
    g_tty_current.c_cc[VMIN] = 0;

    // Update current attributes
    if (tcsetattr(g_FD, TCSANOW, &g_tty_current) == -1)
        return 0;

    return 1;
}

void resetTerminal() {
    tcsetattr(g_FD, TCSANOW, &g_tty_default);
}

enum ArrowKey getMovementKeyPressed() {
    char buff[3];
    size_t count = read(g_FD, buff, 3);
    // Arrow keys always contain 3 bytes; other keys may be different
    if (count != 3)
        return KEY_NONE;
    // Only the last character in the buffer is different between keys
    switch (buff[2]) {
        case 'A': return KEY_UP;
        case 'B': return KEY_DOWN;
        case 'C': return KEY_RIGHT;
        case 'D': return KEY_LEFT;
        default: return KEY_NONE;
    }
}

int getTerminalWidth() {
    return g_winsize.ws_col;
}

int getTerminalHeight() {
    return g_winsize.ws_row;
}
