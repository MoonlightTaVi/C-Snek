#pragma once
#include <stdlib.h>


/** A position in 2D grid. */
typedef struct Cell {
    int x;
    int y;
} Cell;

/** 
 * The struct is basically a dynamic array of CELLs, 
 * with some memory capacity and actual current length 
 * (both represented as the number of CELLs).
 * 
 * The first CELL of the snake is its head; all the others are its tail.
 */
typedef struct CellArray {
    struct Cell* cells;
    size_t length;
    size_t m_capacity;
} CellArray;


/** Returns the first CELL node of the snake. */
struct Cell* getHead(); 

/** Returns the last CELL node of the snake. */
struct Cell* getTail(); 

/** Allocates the memory for the array. */
struct CellArray initCellArray(size_t initial_capacity);

/** 
 * Initializes the snake. 
 * Does not initialize the terminal, because it may be insuccessful.
 */
struct CellArray* initSnake();


/** Frees the occupied memory. Does not delete the snake itself. */
void deleteArray(struct CellArray* arr);

/** Appends a new cell with the given coordinates to the end of the array. */
void append(struct CellArray* arr, int cell_x, int cell_y);

/** 
 * Grows a new CELL at the tip of the snake's tail,
 * spreading the tail towards the given relative direction. 
 */
void growTail(int dx, int dy); 

/**
 * Moves the head of the snake using relative position change;
 * Drags the rest of the snake towards its head;
 * Does not move the snake if it is trying to move towards its tail
 * (backwards).
 */
void moveSnake(int dx, int dy); 

/** Checks if the snake's head is at one of its body cells positions. */
int snakeEatsItself();
