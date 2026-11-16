#include "snek/core.h"


const struct Cell* getHead(struct Snake* snake) {
    return &(snake->cells[0]);
}

const struct Cell* getTail(struct Snake* snake) {
    return &(snake->cells[snake->length - 1]);
}

void createSnake(struct Snake* snake, size_t initialCapacity, int x, int y) {
    if (initialCapacity == 0 || snake->m_capacity != 0) {
        return;
    }

    snake->cells = (struct Cell*) malloc(sizeof(struct Cell) * initialCapacity);
    snake->m_capacity = initialCapacity;
    snake->length = 1;
    snake->cells[0] = (struct Cell) { x, y };
}

void deleteSnake(struct Snake* snake) {
    free(snake->cells);
}

void resize(struct Snake* snake, size_t newCapacity) {
    snake->cells = (struct Cell*) realloc(snake->cells, sizeof(struct Cell) * newCapacity);
    snake->m_capacity = newCapacity;
}

void growTail(struct Snake* snake, int dx, int dy) {
    if (snake->length == snake->m_capacity)
        resize(snake, snake->m_capacity * 2);
    const struct Cell tail = *getTail(snake);
    snake->cells[snake->length++] = (struct Cell) { tail.x + dx, tail.y + dy };
}

void move(struct Snake* snake, int dx, int dy) {
    // Disallow the snake to move into its tail
    if (
        snake->length > 1 
        && snake->cells[0].x + dx == snake->cells[1].x
        && snake->cells[0].y + dy == snake->cells[1].y
       ) { return; } 
    

    // First drag the tail
    for (size_t i = 0; i < snake->length - 1; ++i) {
        size_t j = snake->length - i - 1;
        snake->cells[j].x = snake->cells[j - 1].x; 
        snake->cells[j].y = snake->cells[j - 1].y; 
    }
    // Then update the head's position
    snake->cells[0].x += dx; 
    snake->cells[0].y += dy; 
}
