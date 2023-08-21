/*
Mahyar Mike Vahabi
mvahabi
Winter 22 - CSE101
List.c - List ADTs
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "List.h"

// ****** Citation: Professor Tantalo's implementation of Queue helped with some ADT's *********

// Set "Node" alais to NodeObj
typedef struct NodeObj* Node;

// Private Node object type
typedef struct NodeObj {
    void* data;
    Node left;
    Node right;
} NodeObj;

// Constructor for node
Node node_create(void* data) {
    Node n = malloc(sizeof(NodeObj));
    if (n) {
        n->data = data;
        n->left = NULL;
        n->right = NULL;
    }
    return n;
}

//The destructor for a node
void node_delete(Node* n) {
    if (n && *n) {
        free (*n);
        *n = NULL;
    }
    return;
}

// private List object type
typedef struct ListObj{
    int length;
    int curs_pos; 
    Node curs;
    Node front; 
    Node back;  
}ListObj;

// Constructor for list
List newList(){
    List L = malloc(sizeof(ListObj));
    if (L) {
        L->length = 0;
        L->curs_pos = -1;
        L->curs = NULL;
        L->front = NULL;
        L->back = NULL;
    }
    return L;
}

// Destructor for list 
void freeList(List *L){
    if (!L){
        fprintf(stderr, "Error in FreeList. Can not free an empty list!\n");
        exit(EXIT_FAILURE);
    }
    Node n, temp;
    n = (*L)->front;
    for (int i = 0; i < (*L)->length; i++) {
        temp = n;
        n = n->right;
        free(temp);
    }
    free (*L);
    L = NULL;
}

// Returns the number of elements in L.
int length(List L) {
    if (!L) {
        fprintf(stderr, "Error in length(). List is empty.\n");
        return -1;
    }
    return L->length;
}
// Returns position of curs
int index(List L) {
    if (!L) {
        fprintf(stderr, "Error in index(). List is empty.\n");
       return -1;
    }
    return L->curs_pos;
}
//Returns first element
void* front(List L) {
    if (!L) {
        fprintf(stderr, "Error in front(). List is not defined.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "Error in front(). Length is 0.!\n");
        exit(EXIT_FAILURE);
    }
    return L->front->data;
}
//Returns last element
void* back(List L){
    if (!L) {
        fprintf(stderr, "Error in back(). List is not defined.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "Error in back(). Length is 0.!\n");
        exit(EXIT_FAILURE);
    }
    return L->back->data;
}
// Returns element at cursor 
void* get(List L) {
    if (L) {
        if (L->length > 0 && L->curs_pos >= 0) {
            return &L->curs->data;
        }
    }
    fprintf(stderr, "Error in get(). List is not defined or empty.\n");
    exit(EXIT_FAILURE);
}

// Resets L to its original empty state.
void clear(List L) {
    if (!L) {
        fprintf(stderr, "Error in clear(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    L->length = 0;
    L->curs_pos = -1;
    L->curs = NULL;
    L->front = NULL;
    L->back = NULL;
}
// writes x in the cursor element's data
void set(List L, void* x){
    if (!L) {
        fprintf(stderr, "Error in set(). List is not defined.\n");
        exit(EXIT_FAILURE);
    }
    if (!(L->curs))
    {
        fprintf(stderr, "Error in set(). Cursor is not definded.\n");
        exit(EXIT_FAILURE);
    }
    L->curs->data = x;
}

// Move cursor to the front
void moveFront(List L) {
    if (!L) {
        fprintf(stderr, "Error in moveFront(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length > 0) {
        L->curs = L->front;
        L->curs_pos = 0;
    }
}

// Move cursor to the back
void moveBack(List L) {
    if (!L) {
        fprintf(stderr, "Error in moveBack(). List is empty or length is 0.\n");
        exit(EXIT_FAILURE);
    }
    L->curs_pos = L->length - 1;
    L->curs = L->back;
}


// Move the cursor to one position before it's original position
void movePrev(List L) {
    if (!L) {
        fprintf(stderr, "Error in moveLeft(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    if (L->curs_pos <= 0) {
         L->curs_pos = -1;
         L->curs = NULL;
         return;
    }
    L->curs_pos--;
    L->curs = L->curs->left;
}

// Move the cursor to one position after it's original position
void moveNext(List L) {
    if (!L) {
        fprintf(stderr, "Error in moveRight(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    if(L->curs) {
        L->curs = L->curs->right;
        if(L->curs) {
            L->curs_pos++;
        } else {
            L->curs_pos = -1;
            L->curs = NULL;
        } 
    } 
}

// Add a new node to the front of the list
void prepend(List L, void* data){
    if (!L) {
        fprintf(stderr, "Error in prepend(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    Node new = node_create(data);
    if (L->length == 0){
        L->front = new;
        L->back = L->front;
        L->length++;
    } else {
        new->right = L->front;
        L->front->left = new;
        L->front = new;
        L->length++;
    }
    if (L->curs != NULL) {
        L->curs_pos++;
    }
    return;
}

// Add a new node to the end of the list
void append(List L, void* data) {
    if (!L) {
        fprintf(stderr, "Error in append(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    Node new = node_create(data);
    if (L->length == 0){
        L->front = new;
        L->back = L->front;
        L->length++;
    } else {
        new->left = L->back;
        L->back->right = new;
        L->back = new;
        L->length++;
    }
    return;
}

// Insert new element before cursor
void insertBefore(List L, void* data) {
    if (!L) {
        fprintf(stderr, "Error in insertBefore(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length <= 0) {
        fprintf(stderr, "Error in insertBefore(). Length is 0 or negative.\n");
        exit(EXIT_FAILURE);
    }
    if (L->curs_pos < 0) {
        fprintf(stderr, "Error in insertBefore(). Cursor position is at negative.\n");
        exit(EXIT_FAILURE);
    }
    Node new = node_create(data);
    if(L->curs == L->front) {
        new->right = L->front;
        L->front->left = new;
        L->front = new;
        L->length++;
        L->curs_pos++;
    } else {
        new->left = L->curs->left;
        new->right = L->curs;
        L->curs->left->right = new;
        L->curs->left = new;
        L->length++;
        L->curs_pos++;
    }
    return;
}

// Insert new element after cursor. 
void insertAfter(List L, void* data) {
    if (!L) {
        fprintf(stderr, "Error in insertAfter(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length <= 0) {
        fprintf(stderr, "Error in insertAfter(). Length is 0 or negative.\n");
        exit(EXIT_FAILURE);
    }
    if (L->curs_pos < 0) {
        fprintf(stderr, "Error in insertAfter(). Cursor position is at negative.\n");
        exit(EXIT_FAILURE);
    } 
    Node new = node_create(data);
    if(L->curs == L->back) {
        new->left = L->back;
        L->back->right = new;
        L->back = new;
        L->length++;
    } else { 
        new->right = L->curs->right;
        new->left = L->curs;
        L->curs->right->left = new;
        L->curs->right = new;
        L->length++;
    }
    return;
}

// Delete the first element. 
void deleteFront(List L) {
    if (!L || !(L->front)) {
        fprintf(stderr, "Error in deleteFront(). List is undefined or empty.\n");
        exit(EXIT_FAILURE);
    }
    Node new = NULL;
    if (L->length == 1) {
        node_delete(&L->front);
        L->curs = NULL;
        L->front = NULL;
        L->back = NULL;
        L->curs_pos = -1;
        L->length = 0;
    } else {
        new = L->front;
        L->front = L->front->right;
        node_delete(&new);
        L->front->left = NULL;
        if (L->curs_pos == 0) {
            L->curs = NULL;
        }
        L->curs_pos--;
        L->length--;
    }
}

// Delete the last element.
void deleteBack(List L) {
    if (!L || !(L->back)) {
        fprintf(stderr, "Error in deleteBack(). List is undefined or NULL.\n");
        exit(EXIT_FAILURE);
    }
    Node new = NULL;
    if (L->length == 1) {
        node_delete(&L->front);
        L->curs = NULL;
        L->front = NULL;
        L->back = NULL;
        L->curs_pos = -1;
        L->length = 0;
    } else {
        new = L->back;
        if (L->curs == new) {
            L->curs_pos = -1;
            L->curs = NULL;
        }
        L->back = L->back->left;
        node_delete(&new);
        L->back->right = NULL;
        L->length--;
    }
}

// Delete cursor element
void delete (List L) {
    if (!L) {
        fprintf(stderr, "Error in delete(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    if (!(L->curs)) {
        fprintf(stderr, "Error in delete(). Cursor is undefined.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length > 0 && L->curs_pos == 0) {
        deleteFront(L);
    }
    else if (L->length > 0 && L->curs_pos == (L->length - 1)) {
        deleteBack(L);
    }
    else if (L->length > 0 && L->curs_pos > 0 && L->curs_pos < (L->length - 1)) {
        Node temp = L->curs->right;
        temp->left = L->curs->left;
        temp = L->curs->left;
        temp->right = L->curs->right;
        node_delete(&(L->curs));
        L->curs_pos = -1;
        L->length--;
    }
}

// Prints to the file pointed to by out
/*void printList(FILE *out, List L) {
    if (!L) {
        fprintf(stderr, "Error in printList(). List is empty.\n");
        exit(EXIT_FAILURE);
    }
    Node new = NULL;
    for (new = L->front; new != NULL; new = new->right) { 
        fprintf(out, "%p ", (new->data));
    }
}
*/
