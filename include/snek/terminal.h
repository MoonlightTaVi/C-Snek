#pragma once


/** The color of text. */
enum Color {
    DEFAULT, RED, GREEN
};


/** Clears the entire screen and resets the cursor position. */
void clearScreen();

/**
 * Prints a single character or a sequence of characters 
 * at the given (x;y) position, where X stands for column, and Y for row.
 * 
 * The origin of the screen is at (1;1).
 */
void printAt(int x, int y, const char* c, enum Color color);
