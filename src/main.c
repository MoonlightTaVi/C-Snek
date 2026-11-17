#include <stdio.h>
#include <stdlib.h>
#include "snek/terminal.h"
#include "snek/core.h"


/** Returns a random number in range [from;to). */
int randomRange(int from, int to) {
    return (rand() % (to - from)) + from;
}

/** Places the given cell at a random position on screen. */
void randomizeCoordinates(struct Cell* cell) {
    cell->x = randomRange(1, getTerminalWidth());
    cell->y = randomRange(1, getTerminalHeight());
}

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
void initialize(struct Snake* snake) {
    const int initial_capacity = 8;
    const int initial_length = 4;
    createSnake(
        snake, initial_capacity, 
        initial_length,         // Head X 
        1                       // Head Y
    );
    for (int i = 0; i < initial_length; ++i)
        growTail(snake, -1, 0);
}

/** 
 * Makes the snake move in a loop. Reads player's input. 
 * Quits game if the snake moves out of terminal screen bounds. 
 */
void play(struct Snake* snake) {
    enum ArrowKey move_dir = KEY_RIGHT;
    struct Cell apple;
    randomizeCoordinates(&apple);

    for (int turn = 0; 1; ++turn) {
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
        move(snake, dx, dy);
        // Lose conditions = Quit
        if (!isPointInBoundaries(head->x, head->y))
            break;
        if (eatsItself(snake))
            break;
        // Eat apple
        if (head->x == apple.x && head->y == apple.y) {
            growTail(snake, 0, 0);
            randomizeCoordinates(&apple);
        }
        // Redraw head
        printAt(head->x, head->y, "O", C_GREEN);
        // Print apple, blinking
        printAt(apple.x, apple.y, (turn % 2 == 0 ? "@" : " "), C_RED);
    }
}


int main() {
    printf("WELCOME TO THE SNEK GAME!\n");
    printf("(play as snek and eat a snack)\n\n");
    printf("Press Ctrl+C to QUIT NOW.\n");
    printf("Eat yourself or go outside the screen to QUIT LATER.\n");
    printf("\n\nPress Enter to continue...\n\n");
    getchar();

    if (!initializeTerminalSuccess())
        return 1;

    struct Snake snake;
    initialize(&snake);
    play(&snake);

    clearScreen();
    resetTerminal();
    
    printf("Your final length was %zu snaki-meters. Bye-bye.\n", snake.length);
    deleteSnake(&snake);

    return 0;
}
