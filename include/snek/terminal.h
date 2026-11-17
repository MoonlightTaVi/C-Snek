#pragma once


/** The color of text. */
enum Color {
    C_DEFAULT, C_RED, C_GREEN
};

/** 
 * Represents an arrow 'key pressed' event for basic movement input,
 * as well as 'no key/some other key pressed' default fallback.
 */
enum ArrowKey {
    KEY_NONE, KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT 
};


/** Clears the entire screen and resets the cursor position. */
void clearTerminalScreen();

/**
 * Prints a single character or a sequence of characters 
 * at the given (x;y) position, where X stands for column, and Y for row.
 * 
 * The origin of the screen is at (1;1).
 */
void printAt(int x, int y, const char* c, enum Color color);

/** 
 * Initializes the terminal application 
 * by enabling raw mode and disabling input echo.
 * Returns 1 if the initialization was successful, 0 otherwise.
 * 
 * The terminal must be manually reset to its default state when
 * application quits.
*/
int initializeTerminalSuccess(); 

/** Resets the terminal to its default state. */
void resetTerminalToDefaults(); 

/** Returns whether (and which) arrow key was recently pressed by the user. */
enum ArrowKey getMovementKeyPressed();

/** Returns the width of the terminal, in symbols. */
int getTerminalScreenWidth();

/** Returns the height of the terminal, in symbols. */
int getTerminalScreenHeight();

/** Returns 1 if the point is inside the terminal's screen boundaries; 0 if it is outside. */
int isPointInBoundaries(int x, int y); 
