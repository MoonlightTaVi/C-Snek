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
struct Snake {
    struct Cell* cells;
    size_t length;
    size_t m_capacity;
};


/** Returns the first CELL node of the snake. */
const struct Cell* getHead(struct Snake* snake); 

/** Returns the last CELL node of the snake. */
const struct Cell* getTail(struct Snake* snake); 

/** 
 * Creates a new snake with the given memory capacity, length of one CELL,
 * with its head at the given position. 
 */
void createSnake(struct Snake* snake, size_t initialCapacity, int x, int y); 

/** Frees the occupied memory. Does not delete the snake itself. */
void deleteSnake(struct Snake* snake);

/** 
 * Reallocates the internal memory to hold new capacity;
 * capacity is the number of CELLs.
 */
void resize(struct Snake* snake, size_t newCapacity); 

/** 
 * Grows a new CELL at the tip of the snake's tail,
 * spreading the tail towards the given relative direction; 
 * the method is only useful right after the snake creation. 
 */
void growTail(struct Snake* snake, int dx, int dy); 

/**
 * Moves the head of the snake using relative position change;
 * Drags the rest of the snake towards its head;
 * Does not move the snake if it is trying to move towards its tail
 * (backwards).
 */
void move(struct Snake* snake, int dx, int dy); 
