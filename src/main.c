#include <stdio.h>
#include "snek/terminal.h"
#include "snek/core.h"


/** Prints debugging information about snake properties. */
void debug(struct Snake* snake) {
    if (snake == NULL)
        return;
    printf("Snake[length=%zu;capacity=%zu]\n", snake->length, snake->m_capacity);
    for (size_t i = 0; i < snake->length; ++i) {
        printf("(%d;%d) ", snake->cells[i].x, snake->cells[i].y);
    }
    printf("\n");
}

/** 
 * Initializes the snake. 
 * Does not initialize the terminal, because it may be insuccessful.
 */
void begin(struct Snake* snake) {
    const int initial_length = 4;
    createSnake(snake, 8, initial_length, 1);
    for (int i = 0; i < initial_length; ++i)
        growTail(snake, -1, 0);
}

/** Deletes the snake and clears the screen. */
void end(struct Snake* snake) {
    deleteSnake(snake);
    clearScreen();
}

/** 
 * Makes the snake move in a loop. Reads player's input. 
 * Quits game if the snake moves out of terminal screen bounds. 
 */
void play(struct Snake* snake) {
    enum ArrowKey last_key = KEY_RIGHT;
    while (1) {
        enum ArrowKey key = getMovementKeyPressed();
        if (key != KEY_NONE)
            last_key = key;
        int dx = (int)(last_key == KEY_RIGHT) - (int)(last_key == KEY_LEFT);
        int dy = (int)(last_key == KEY_DOWN)  - (int)(last_key == KEY_UP);

        const struct Cell *head = getHead(snake);
        const struct Cell *tail = getTail(snake);
        // Erase tail and reset head's symbol to body symbol
        printAt(tail->x, tail->y, ".", C_DEFAULT);
        printAt(head->x, head->y, "o", C_GREEN);
        // Change head position
        move(snake, dx, dy);
        // Lose conditions = Quit
        if (!isPointInBoundaries(head->x, head->y))
            break;
        if (eatsItself(snake))
            break;
        // Redraw head
        printAt(head->x, head->y, "O", C_GREEN);
    }
}

int main() {
    if (!initializeTerminalSuccess())
        return 1;

    struct Snake snake;
    begin(&snake);
    play(&snake);
    end(&snake);

    resetTerminal();
    return 0;
}
