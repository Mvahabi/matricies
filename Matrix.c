/*
Mahyar Mike Vahabi
mvahabi
Winter 22 - CSE101
Matrix.c - Matrix ADTs
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "Matrix.h"
#include "List.h"

// Set "Entry" alais to EntryObj
typedef struct EntryObj *Entry;

// private Entry Object type
typedef struct EntryObj {
    int col;
    double val;
} EntryObj;

typedef struct MatrixObj {
    int size;
    int NNZ;
    List *list;
} MatrixObj;

// constructor for entry
Entry newEntry(int column, double value) {
    if (column < 0) {
        fprintf(stderr, "Error in newEntry(). Invalid column.\n");
        return NULL;
    }
    Entry E = malloc(sizeof(EntryObj));
    if (E) {
        E->col = column;
        E->val = value;
    }
    return E;
}

void deleteEntry(Entry *pE) {
    if (pE && *pE) {
        free(*pE);
        *pE = NULL;
    }
}

// constructor for matrix
Matrix newMatrix(int n) {
    if (n < 0) {
        fprintf(stderr, "Error in newMatrix function, n is less than 0\n");
        exit(EXIT_FAILURE);
    }
    Matrix M = malloc(sizeof(MatrixObj));
    if (M) {
        M->size = n;
        M->NNZ = 0;
        M->list = malloc(sizeof(List) * n + 1);
        for (int i = 1; i <= n; i++) {
            M->list[i] = newList();
        }
    }
    return M;
}

// destructor for Matrix
void freeMatrix(Matrix* pM) {
    if (!pM) {
        fprintf(stderr, "Error in freeMaatrix(). Matrix doesn't exists.");
        exit(EXIT_FAILURE);
    }
    if (pM && *pM) {
        makeZero(*pM);
        free(*pM);
        *pM = NULL;
    }
}

// Return the size of square Matrix M.
int size(Matrix M) {
    if(!M){
        fprintf(stderr, "Error in size(). Empty Matrix.\n");
        exit(EXIT_FAILURE);
    }
    return M->size;
}

// Return the number of non-zero elements in M.
int NNZ(Matrix M) {
    if(!M) {
        fprintf(stderr, "Error in NNZ(). Empty Matrix.\n");
        exit(EXIT_FAILURE);
    }
    return M->NNZ;
}


// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B) {
    if(!A || !B) {
        fprintf(stderr, "Error in equals(). Empty Matrices.\n");
        return 0;
    }
    if (A->size != B->size) {
        fprintf(stdout, "Matrix A's size does not equal Matrix B's size\n");
        return 0;
    }
    //fprintf(stdout, "size(A): %d\n", size(A));
    for (int i = 1; i <= size(A); i++) {
        Entry x;
        Entry y;
        moveFront(A->list[i]);
        moveFront(B->list[i]);
        if (length(A->list[i]) != length(B->list[i])) {
            return 0;
        }
        while (index(A->list[i]) >= 0 && index(B->list[i]) >= 0) {
            x = *(Entry*)get(A->list[i]);
            y = *(Entry*)get(B->list[i]);
            if (x->val != y->val){
                return 0;
            }
            moveNext(A->list[i]);
            moveNext(B->list[i]);
        }
    }
    return 1;
}

// Re-sets M to the zero Matrix state.
void makeZero(Matrix M) {
    if(!M) {
      fprintf(stderr, "Error in makeZero(). Empty Matrix.\n");
      exit(EXIT_FAILURE);
    }
    M->NNZ = 0;
    for (int i = 1; i <= size(M); i++) {
        moveFront(M->list[i]);
        clear((M->list[i]));
    }
}

// Changes the ith list, jth col of M to the val x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
    if (!M) {
        fprintf(stderr, "Error in changeEntry(). Empty Matrix.\n");
        exit(EXIT_FAILURE);
    }
    if (i < 1 || i > size(M) || j < 1 || j > size(M)) {
        fprintf(stdout, "Matrix out of range.\n");
        return;        
    }
    Entry new = newEntry(j, x);
    Entry E = newEntry(0, 0);
    int cont = 0;
    if (length(M->list[i]) == 0) {
        if (x != 0) {
            append(M->list[i], new);
            M->NNZ++;
        }
    }
    else {
        moveFront(M->list[i]);
        while(index(M->list[i])>=0 ){
            E = *(Entry*)get(M->list[i]);
            if (new->col == E->col) {
                if (x == 0) {
                    delete(M->list[i]);
                    cont = 1;
                    M->NNZ--;
                    break;
                }
                else if (x != 0) {
                    set(M->list[i], new);
                    cont = 1;
                    break;
                }
            }
            else if (new->col < E->col) {
                if (x != 0) {
                    insertBefore(M->list[i], new);
                    cont = 1;
                    M->NNZ++;
                    break;
                }
            }
            moveNext(M->list[i]);
        }
        if (cont == 0 && x != 0) {
            append(M->list[i], new);
            M->NNZ++;
        }
    }
}

// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A) {
    if (!A) {
        fprintf(stderr, "Error in copy(). Empty Matrix.\n");
        Matrix copy = newMatrix(0);
        return copy;
    }
    Matrix copy = newMatrix(size(A));
    Entry new;
    Entry E;
    copy->NNZ = A->NNZ;
    for (int i = 1; i <= size(A); i++) {
        moveFront(A->list[i]);
        while (index(A->list[i]) >= 0) {
            E = *(Entry*)get(A->list[i]);
            new = newEntry(E->col, E->val);
            append(copy->list[i], new);
            moveNext(A->list[i]);
        }
    }
    return copy;
}

//Returns a reference to a new Matrix object representing the transpose of A.
Matrix transpose(Matrix A) {
    if(!A) {
      fprintf(stderr, "Error in Transpose(). Empty Matrix.\n");
      exit(EXIT_FAILURE);
    }
    Matrix Trans = newMatrix(size(A));
    Entry new;
    Entry E;
    Trans->NNZ = A->NNZ;
    for (int i = 1; i <= size(A); i++) {
        moveFront(A->list[i]);
        while (index(A->list[i]) >= 0) {
            E = *(Entry*)get(A->list[i]);
            new = newEntry(i, E->val);
            append(Trans->list[E->col], new);
            moveNext(A->list[i]);
        }
    }
    return Trans;
}

// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A) {
    if (!A) {
        fprintf(stderr, "Error in scalarMuly(). Empty Matrix.\n");
        exit(EXIT_FAILURE);
    }
    Matrix mul = newMatrix(size(A));
    Entry new;
    Entry E;
    if (x == 0 || mul == NULL) {
        return mul;
    }
    mul->NNZ = A->NNZ;
    for (int i = 1; i <= size(A); i++) {
        moveFront(A->list[i]);
        while (index(A->list[i]) >= 0) {
            E = *(Entry*)get(A->list[i]);
            new = newEntry(E->col, (x * E->val));
            append(mul->list[i], new);
            moveNext(A->list[i]);
        }
    }
    return mul;
}

// helper function for sum, diff later
List add(List A, List B, int x, Matrix CC) {
    moveFront(A);
    moveFront(B);
    Entry a, b, c, d;
    List C = newList();
    int aa = 0;
    int bb = 0;
    while (index(A) >= 0 && index(B) >= 0) {
        a = *(Entry*)get(A);
        b = *(Entry*)get(B);
        if (a->col == b->col) {
            if (x == 1) c = newEntry(a->col, (a->val + b->val));
            if (x == -1) c = newEntry(a->col, (a->val - b->val));
            if (c->val != 0) {
                CC->NNZ++;
                append(C, c);
            }
            moveNext(A);
            moveNext(B);
            if (index(A) < 0) aa = 1;
            if (index(B) < 0) bb = 1;
        }
        else if (a->col < b->col) {
            c = newEntry(a->col, a->val);
            if (c->val != 0) {
                CC->NNZ++;
                append(C, c);
            }
            moveNext(A);
            if (index(A) < 0) aa = 1;
        }
        else {
            if (x == 1) c = newEntry(b->col, b->val);
            if (x == -1) c = newEntry(b->col, -(b->val));
            if (c->val != 0) {
                CC->NNZ++;
                append(C, c);
            }
            moveNext(B);
            if (index(B) < 0) bb = 1;
        }
    }
    if (aa == 0) {
        while (index(A) >= 0) {
            c = *(Entry*)get(A);
            d = newEntry(c->col, c->val);
            if (d->val != 0) {
                CC->NNZ++;
                append(C, d);
            }
            moveNext(A);
        }
    }
    if (x == 1) {
        if (bb == 0) {
            while (index(B) >= 0) {
                c = *(Entry*)get(B);
                d = newEntry(c->col, c->val);
                if (d->val != 0) {
                    CC->NNZ++;
                    append(C, d);
                }
                moveNext(B);
            }
        }
    }
    else if (x == -1) {
        if (bb == 0) {
            while (index(B) >= 0) {
                c = *(Entry*)get(B);
                d = newEntry(c->col, -(c->val));
                if (d->val != 0) {
                    CC->NNZ++;
                    append(C, d);
                }
                moveNext(B);
            }
        }
    }
    return C;
}

// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) {
    if (!A || !B) {
        fprintf(stdout, "Error in sum(). Empty matrices.\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size) {
        fprintf(stdout, "Matrix A's size does not equal Matrix B's size\n");
        exit(EXIT_FAILURE);
    }
    Matrix Sum = newMatrix(size(A));
    if (A == B) {
        Sum = scalarMult((double) 2, A);
        Sum->NNZ = A->NNZ;
        return Sum;
    }
    for (int i = 1; i <= size(A); i++) {
        Sum->list[i] = add(A->list[i], B->list[i], 1, Sum);
    }
    return Sum;
}

// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {
    if (!A || !B) {
        fprintf(stdout, "Error in diff(). Empty matrices.\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size) {
        fprintf(stdout, "Error in diff(). Sizes are unequall.\n");
        exit(EXIT_FAILURE);
    }
    Matrix Diff = newMatrix(size(A));
    if (A == B) {
        Diff = copy(A);
        makeZero(Diff);
        return Diff;
    }
    for (int i = 1; i <= size(A); i++) {
        Diff->list[i] = add(A->list[i], B->list[i], -1, Diff);
    }
    return Diff; 
}

Entry mult(List A, List B, int col) {
    moveFront(A);
    moveFront(B);
    Entry a, b, c;
    double Mult = 0;
    while (index(A) >= 0 && index(B) >= 0) {
        a = *(Entry*)get(A);
        b = *(Entry*)get(B);
        if (a->col == b->col) {
            Mult += (a->val * b->val);
            moveNext(A);
            moveNext(B);
        }
        else if (a->col < b->col) {
            moveNext(A);
        }
        else {
            moveNext(B);
        }
    }
    c = newEntry(col, Mult);
    return c;
}

// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B) {
    if (!A || !B) {
        fprintf(stdout, "Error in product(). Empty matrices.\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size) {
        fprintf(stdout, "Error in product(). Sizes are unequall.\n");
        exit(EXIT_FAILURE);
    }
    Matrix Trans = transpose(B);
    Matrix Prod = newMatrix(size(A));
    Entry new = newEntry(0, 0.0);
    for (int i = 1; i <= size(A); i++) {
        for (int j = 1; j <= size(A); j++) {
            if ((length(A->list[i]) > 0) && (length(Trans->list[j]) > 0)) {
                new = mult(A->list[i], Trans->list[j], j);
                if (new->val != 0) {
                    append(Prod->list[i], new);
                    Prod->NNZ++;
                }
            }
        }
    }
    freeMatrix(&Trans);
    return Prod;
}

// printMatrix()
void printMatrix(FILE* out, Matrix M) {
    Entry E = newEntry(0, 0);
    int cont = 0;
    for (int i = 1; i <= size(M); i++) {
        cont = 0;
        moveFront((M->list[i]));
        if (index(M->list[i]) >= 0) {
            E = *((Entry*) get(M->list[i]));
            if (E) {
                fprintf(out, "%d: ", i);
                cont = 1;
            }
        }
        while (index((M->list[i])) >= 0) {
            E = *((Entry*) get(M->list[i]));
            if (E) {
                fprintf(out, "(%d, %.1f) ", E->col, E->val);
            }
            moveNext(M->list[i]);
        }
        if (cont == 1) {
            fprintf(out, "\n");
        }
    }
}
