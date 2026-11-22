// Window resized signal handling.
// Sadly, it requires POSIX standard from September, 2008.
// But without it, the game may fall into undefined behaviour (possibly crash),
// And we can't forbid the user to resize the terminal.
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
// General imports
#include "snek/terminal.h"
#include <unistd.h>
// Terminal IO controls
#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>


/** Erases the screen and resets cursor position. */
#define ANSI_CLEAR "\033[2J\033[H"
/** Plays notification sound, if supported. */
#define ANSI_BELL "\a"

/** Hides the terminal input cursor. */
#define ANSI_CURSOR_HIDE "\033[?25l"
/** Shows the terminal input cursor. */
#define ANSI_CURSOR_SHOW "\033[?25h"

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
/** Performs an action when window is resized. */
struct sigaction g_sa;

/**
 * When set to 1, indicates that the window has been just resized.
 * Must be reset to 0 manually.
 */
volatile sig_atomic_t g_win_resized = 0;

/** Updates the g_win_resized flag to indicate the window resize. */
void handleWindowResizedSignal(int signal) {
    if (signal != SIGWINCH)
        return;
    g_win_resized = 1;
}


int checkWindowResized() {
    if (!g_win_resized)
        return 0;
    ioctl(g_FD, TIOCGWINSZ, &g_winsize);
    g_win_resized = 0;
    printf(ANSI_BELL);
    printf(ANSI_CLEAR);
    return 1;
}

int getTerminalScreenWidth() {
    return g_winsize.ws_col;
}

int getTerminalScreenHeight() {
    return g_winsize.ws_row;
}

int isPointInBoundaries(int x, int y) {
    return x > 0 && y > 0 && x <= getTerminalScreenWidth() && y <= getTerminalScreenHeight();
}


/** @return A string representation of ANSI code for this enum. */
const char* getColor(enum Color c) {
    switch (c) {
        case C_RED: return COLOR_RED;
        case C_GREEN: return COLOR_GREEN;
        default: return COLOR_DEF;
    }
}


void clearTerminalScreen() {
    printf(ANSI_CLEAR);
}

void printAt(int x, int y, const char* c, enum Color color) {
    if (!isPointInBoundaries(x, y))
        return;
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
    // Disable STDOUT buffering (only for this process)
    setvbuf(stdout, NULL, _IONBF, 0);
    // Read current attributes
    if (tcgetattr(g_FD, &g_tty_current) == -1)
        return 0;    
    if (tcgetattr(g_FD, &g_tty_default) == -1)
        return 0;    
    // Enable non-canonical (raw) mode with no echo; disable ^C termination
    g_tty_current.c_lflag &= ~ICANON;
    g_tty_current.c_lflag &= ~ECHO;
    g_tty_current.c_lflag &= ~ISIG;
    g_tty_current.c_cc[VTIME] = 1;
    g_tty_current.c_cc[VMIN] = 0;
    // Update current attributes
    if (tcsetattr(g_FD, TCSANOW, &g_tty_current) == -1)
        return 0;
    // Handle window resize signal
    g_sa.sa_handler = handleWindowResizedSignal;
    sigemptyset(&g_sa.sa_mask);
    g_sa.sa_flags = SA_RESTART;
    sigaction(SIGWINCH, &g_sa, NULL);

    printf(ANSI_CURSOR_HIDE);
    return 1;
}

void resetTerminalToDefaults() {
    tcsetattr(g_FD, TCSANOW, &g_tty_default);
    printf(ANSI_CURSOR_SHOW);
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

/** @return The key that represents the opposite direction to the given argument, or KEY_NONE. */
enum ArrowKey getOppositeOf(enum ArrowKey direction) {
    switch (direction) {
        case KEY_DOWN: return KEY_UP;
        case KEY_UP: return KEY_DOWN;
        case KEY_LEFT: return KEY_RIGHT;
        case KEY_RIGHT: return KEY_LEFT;
        default: return KEY_NONE;
    }
}

int areOppositeDirections(enum ArrowKey a, enum ArrowKey b) {
    if (a == KEY_NONE || b == KEY_NONE)
        return 0;
    return getOppositeOf(a) == b;
}
