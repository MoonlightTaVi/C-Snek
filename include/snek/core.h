#pragma once
#include <stdlib.h>


/** A position in 2D grid. */
struct Cell {
    int x;
    int y;
};

/** 
 * The struct is basically a dynamic array of CELLs, 
 * with some memory capacity and actual current length 
 * (both represented as the number of CELLs).
 * 
 * The first CELL of the snake is its head; all the others are its tail.
 */
struct CellArray {
    struct Cell* cells;
    size_t length;
    size_t m_capacity;
};


/** Returns the first CELL node of the snake. */
struct Cell* getHead(struct CellArray* snake); 

/** Returns the last CELL node of the snake. */
struct Cell* getTail(struct CellArray* snake); 

/** Allocates the memory for the array. */
void initCellArray(struct CellArray* arr, size_t initial_capacity);


/** Frees the occupied memory. Does not delete the snake itself. */
void deleteArray(struct CellArray* snake);

/** 
 * Reallocates the internal memory to hold new capacity;
 * capacity is the number of CELLs.
 */
void resize(struct CellArray* snake, size_t new_capacity); 

/** Appends a new cell with the given coordinates to the end of the array. */
void append(struct CellArray* arr, int cell_x, int cell_y);

/** 
 * Grows a new CELL at the tip of the snake's tail,
 * spreading the tail towards the given relative direction. 
 */
void growTail(struct CellArray* snake, int dx, int dy); 

/**
 * Moves the head of the snake using relative position change;
 * Drags the rest of the snake towards its head;
 * Does not move the snake if it is trying to move towards its tail
 * (backwards).
 */
void moveSnake(struct CellArray* snake, int dx, int dy); 

/** Checks if the snake's head is at one of its body cells positions. */
int eatsItself(struct CellArray* snake);
