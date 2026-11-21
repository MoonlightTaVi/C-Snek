#include "snek/core.h"


const struct Cell* getHead(struct CellArray* snake) {
    return &(snake->cells[0]);
}

const struct Cell* getTail(struct CellArray* snake) {
    return &(snake->cells[snake->length - 1]);
}

void initCellArray(struct CellArray* arr, size_t initial_capacity) {
    if (initial_capacity == 0) {
        return;
    }

    arr->cells = (struct Cell*) malloc(sizeof(struct Cell) * initial_capacity);
    arr->m_capacity = initial_capacity;
    arr->length = 0;
}

void deleteArray(struct CellArray* snake) {
    free(snake->cells);
}

void resize(struct CellArray* snake, size_t new_capacity) {
    snake->cells = (struct Cell*) realloc(snake->cells, sizeof(struct Cell) * new_capacity);
    snake->m_capacity = new_capacity;
}

/** Calculates the first free index of the array to append; resizes the array if needed. */
size_t newIndex(struct CellArray* arr) {
    if (arr->length == arr->m_capacity)
        resize(arr, arr->m_capacity * 2);
    return (arr->length)++;
}

void append(struct CellArray* arr, int cell_x, int cell_y) {
    arr->cells[newIndex(arr)] = (struct Cell) { cell_x, cell_y };
}

void growTail(struct CellArray* snake, int dx, int dy) {
    if (snake->length == snake->m_capacity)
        resize(snake, snake->m_capacity * 2);
    const struct Cell tail = *getTail(snake);
    snake->cells[snake->length++] = (struct Cell) { tail.x + dx, tail.y + dy };
}

void moveSnake(struct CellArray* snake, int dx, int dy) {
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

int eatsItself(struct CellArray* snake) {
    const struct Cell* head = getHead(snake);
    for (size_t i = 1; i < snake->length; ++i) {
        if (snake->cells[i].x == head->x && snake->cells[i].y == head->y)
            return 1;
    }
    return 0;
}
