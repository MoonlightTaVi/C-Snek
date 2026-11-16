#include <stdio.h>
#include "snek/terminal.h"
#include "snek/core.h"


void debug(struct Snake* snake) {
    if (snake == NULL)
        return;
    printf("Snake[length=%zu;capacity=%zu]\n", snake->length, snake->m_capacity);
    for (size_t i = 0; i < snake->length; ++i) {
        printf("(%d;%d) ", snake->cells[i].x, snake->cells[i].y);
    }
    printf("\n");
}


int main() {
    struct Snake snake;
    createSnake(&snake, 3, 1, 1);
    
    growTail(&snake, 1, 0);
    growTail(&snake, 1, 0);
    growTail(&snake, 1, 0);
    growTail(&snake, 1, 0);
    growTail(&snake, 1, 0);
    growTail(&snake, 1, 0);
    debug(&snake);

    deleteSnake(&snake);
    return 0;
}
