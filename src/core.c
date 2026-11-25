#include "snek/core.h"

#ifdef T
#undef T
#endif
#define T Cell
#include "snek/linkedlist.h"


/** When 1, indicates that the snake has been created and was not dosposed yet. */
int g_is_initialized = 0;
/** The only snake struct that may be present in the game. */
struct CellArray g_snake;
/** The same playable snake, but as a linked list. For optimization purposes. */
LinkedList_Cell g_linked_snake;


struct Cell* getHead() {
    return &g_snake.cells[0];
    //return g_linked_snake.head->cell;
}

struct Cell* getTail() {
    return &g_snake.cells[g_snake.length - 1];
    //return g_linked_snake.tail->cell;
}

struct CellArray initCellArray(size_t initial_capacity) {
    if (initial_capacity == 0)
        initial_capacity = 8;
    return (struct CellArray) {
        malloc(sizeof(struct Cell) * initial_capacity),
        0,
        initial_capacity
    };
}

struct CellArray* initSnake() {
    if (g_is_initialized)
        goto hell;
    g_is_initialized = 1;

    const int initial_capacity = 8;
    const int initial_length = 4;

    g_snake = initCellArray(initial_capacity);
    append(&g_snake, initial_length, 1);
    // The first linked list element must be added manually:
    g_linked_snake = initList_Cell(&g_snake.cells[0]);

    for (int i = 0; i < initial_length; ++i) {
        // Also writes to the linked list:
        growTail(-1, 0);
    }
    
    hell:
    return &g_snake;
}

void deleteArray(struct CellArray* arr) {
    if (arr == &g_snake) {
        g_is_initialized = 0;
        eraseNodes_Cell(&g_linked_snake);
    }
    free(arr->cells);
}

void resize(struct CellArray* arr, size_t new_capacity) {
    arr->cells = (struct Cell*) realloc(arr->cells, sizeof(struct Cell) * new_capacity);
    arr->m_capacity = new_capacity;
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

void growTail(int dx, int dy) {
    if (g_snake.length == g_snake.m_capacity)
        resize(&g_snake, g_snake.m_capacity * 2);
    struct Cell* tail = getTail(g_snake);
    append(&g_snake, tail->x + dx, tail->y + dy);
    addNode_Cell(&g_linked_snake, &g_snake.cells[g_snake.length - 1]);
}

void moveSnake(int dx, int dy) {
    // First drag the tail
    for (size_t i = 0; i < g_snake.length - 1; ++i) {
        size_t j = g_snake.length - i - 1;
        g_snake.cells[j].x = g_snake.cells[j - 1].x; 
        g_snake.cells[j].y = g_snake.cells[j - 1].y; 
    }
    // Then update the head's position
    g_snake.cells[0].x += dx; 
    g_snake.cells[0].y += dy; 
}

int snakeEatsItself() {
    struct Cell* head = getHead(g_snake);
    for (size_t i = 1; i < g_snake.length; ++i) {
        if (g_snake.cells[i].x == head->x && g_snake.cells[i].y == head->y)
            return 1;
    }
    return 0;
}
