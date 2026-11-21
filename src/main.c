#include "snek/terminal.h"
#include "snek/core.h"
#include <stdio.h>
#include <stdlib.h>


/** Returns a random number in range [from;to). */
int randomRange(int from, int to) {
    return (rand() % (to - from)) + from;
}

/** Places the given cell at a random position on screen. */
void randomizeCoordinates(struct Cell* cell) {
    cell->x = randomRange(1, getTerminalScreenWidth());
    cell->y = randomRange(1, getTerminalScreenHeight());
}

/** Prints debugging information about snake properties. */
void debug(struct CellArray* snake) {
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
void initialize(struct CellArray* snake) {
    const int initial_capacity = 8;
    const int initial_length = 4;
    initCellArray(snake, initial_capacity);
    snake->length = initial_length;
    snake->cells[0] = (struct Cell) { initial_length, 1 }; // { X, Y }
    for (int i = 0; i < initial_length; ++i)
        growTail(snake, -1, 0);
}

/** 
 * Makes the snake move in a loop. Reads player's input. 
 * Quits game if the snake moves out of terminal screen bounds. 
 */
void play(struct CellArray* snake) {
    const int LVL_UP_LENGTH = 10;
    enum ArrowKey move_dir = KEY_RIGHT;

    struct CellArray apples;
    initCellArray(&apples, 6);
    apples.cells[newIndex(&apples)] = (struct Cell) {
        randomRange(1, getTerminalScreenWidth()),
        randomRange(1, getTerminalScreenHeight())
    };

    for (int turn = 0; 1; ++turn) {
        checkWindowResized();

        enum ArrowKey key_press = getMovementKeyPressed();
        if (key_press != KEY_NONE)
            move_dir = key_press;
        int dx = (int)(move_dir == KEY_RIGHT) - (int)(move_dir == KEY_LEFT);
        int dy = (int)(move_dir == KEY_DOWN)  - (int)(move_dir == KEY_UP);

        const struct Cell *head = getHead(snake);
        const struct Cell *tail = getTail(snake);

        // Erase tail and reset head's symbol to body symbol
        printAt(tail->x, tail->y, " ", C_DEFAULT);
        printAt(head->x, head->y, "o", C_GREEN);
        // Change head position
        moveSnake(snake, dx, dy);
        // Lose conditions = Quit
        if (!isPointInBoundaries(head->x, head->y))
            break;
        if (eatsItself(snake))
            break;
        // Process apples
        for (size_t i = 0; i < apples.length; ++i) {
            // Render
            struct Cell* apple = &(apples.cells[i]);
            printAt(apple->x, apple->y, (turn % 3 != 0 ? "@" : " "), C_RED);
            // Eat
            if (head->x != apple->x || head->y != apple->y)
                continue;
            // Grow
            growTail(snake, 0, 0);
            randomizeCoordinates(apple);
            // Increase number of apples as a LVL UP
            // Each 10th length unit adds 1 apple
            if (apples.length * LVL_UP_LENGTH < snake->length) {
                apples.cells[newIndex(&apples)] = (struct Cell) {
                    randomRange(1, getTerminalScreenWidth()),
                    randomRange(1, getTerminalScreenHeight())
                };
            }
        }
        // Redraw head
        printAt(head->x, head->y, "O", C_GREEN);
    }

    deleteArray(&apples);
}


int main() {
    printf("WELCOME TO THE SNEK GAME!\n");
    printf("(play as snek and eat a snack)\n\n");
    printf("Press Ctrl+C to QUIT NOW.\n");
    printf("Eat yourself or go outside the screen to QUIT LATER.\n");
    printf("\n\nPress Enter to continue...\n\n");
    getchar();

    if (!initializeTerminalSuccess())
        return EXIT_FAILURE;

    struct CellArray snake;
    initialize(&snake);
    play(&snake);

    clearTerminalScreen();
    resetTerminalToDefaults();
    
    printf("Your final length was %zu sneki-meters. Bye-bye.\n", snake.length);
    deleteArray(&snake);

    return EXIT_SUCCESS;
}
