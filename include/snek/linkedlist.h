/** A linked list template for general utility usage. */

#ifdef T

#include "snek/templates.h"
#include <stdlib.h>


/** A node of a linked list. Contains the reference to its next neighbour (NULL by default). */
typedef struct TEMPLATE(Node,T) {
    T* value;
    struct TEMPLATE(Node,T)* next;
} TEMPLATE(Node,T);

/** A linked list; holds the information about its first and last element and its size. */
typedef struct TEMPLATE(LinkedList,T) {
    TEMPLATE(Node,T)* first;
    TEMPLATE(Node,T)* last;
    size_t size;
} TEMPLATE(LinkedList,T);


/** 
 * A helper method that wraps a value into the corresponding node type. 
 * The returned node is allocated dynamically. 
 */
static TEMPLATE(Node,T)* TEMPLATE(createNode,T)(T* node_value) {
    TEMPLATE(Node,T)* new_node = malloc(sizeof(TEMPLATE(Node,T)));
    if (!new_node)
        return NULL;
    new_node->value = node_value;
    new_node->next = NULL;
    return new_node;
}

/** Initializes a new linked list with the given first element. */
TEMPLATE(LinkedList,T) TEMPLATE(initList,T)(T* first_value) {
    TEMPLATE(Node,T)* first_node = TEMPLATE(createNode,T)(first_value);
    return (TEMPLATE(LinkedList,T)) { first_node, first_node, 1 };
}

/** 
 * Adds a new element to the given list. 
 * May be used to re-fill the list, the elements of which were previously erased.
 */
void TEMPLATE(addNode,T)(TEMPLATE(LinkedList,T)* to_list, T* node_value) {
    TEMPLATE(Node,T)* new_node = TEMPLATE(createNode,T)(node_value);

    // A completely empty list
    if (to_list->last == NULL) {
        to_list->first = new_node;
        to_list->last = new_node;
    }
    // A list with some elements in it
    else {
        to_list->last->next = new_node;
        to_list->last = new_node;
    }
    
    to_list->size++;
}

/** 
 * Frees all of the memory occupied by the list nodes. 
 * The list itself may be re-used. 
 */
void TEMPLATE(eraseNodes,T)(TEMPLATE(LinkedList,T)* from_list) {
    if (from_list->first == NULL || from_list->last == NULL)
        return;
    TEMPLATE(Node,T)* node = from_list->first;

    // Only one element
    if (from_list->first == from_list->last) {
        free(node);
        goto hell;
    }
    // A few elements
    while (node != NULL) {
        TEMPLATE(Node,T)* temp = node->next;
        free(node);
        node = temp;
    }

    hell:
    from_list->size = 0;
    from_list->first = NULL;
    from_list->last = NULL;
}

#endif
