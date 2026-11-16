#include "snek/terminal.h"


int main() {
    clearScreen();
    // Returns 0 at failure
    if (!initializeTerminalSuccess()) {
        return 1;
    }
    // Print a number from 0 to 4 at position (1;1) three times
    for (int i = 0; i < 3; ++i) {
        char c = '0' + getMovementKeyPressed();
        printAt(1, 1, &c, DEFAULT);
    }

    resetTerminal();
    return 0;
}
