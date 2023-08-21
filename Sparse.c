#include <stdio.h>
#include "Matrix.h"

int main (int argc, char* argv[]) {
    int n, i, nnza, nnzb, row, col;
    double val;
    FILE* in; 
    FILE* out; 
    Matrix A, B, C, D, E, F, G, H, I, J;

    // check command line for correct number of arguments
    if( argc != 3 ) {
	printf("Usage: %s <input file> <output file>\n", argv[0]);
	exit(1);
    }

    // open input file for reading
    in = fopen(argv[1], "r");
    if(!in) {
	printf("Unable to read from file %s\n", argv[1]);
	exit(1);
    }

    // open output file for writing
    out = fopen(argv[2], "w");
    if(!out) {
	printf("Unable to write to file %s\n", argv[2]);
	exit(1);
    }

    // scan iand init Matrix 
    fscanf(in, " %d %d %d", &n, &nnza, &nnzb);

    A = newMatrix(n);
    B = newMatrix(n);

    for(i=0; i < nnza; i++) {
	fscanf(in, " %d %d %lf", &row, &col, &val);
        //fprintf(stdout, "before change entry\n");
	changeEntry(A, row, col, val);
        //fprintf(stdout, "after change entry\n");
    }

    for(i=0; i < nnzb; i++){
        fscanf(in, " %d %d %lf", &row, &col, &val);
        changeEntry(B, row, col, val);
    }

    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);

    fprintf(out, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    
    fprintf(out, "\n(1.5)*A =\n");
    C = scalarMult(1.5, A);
    printMatrix(out, C);

    fprintf(out, "\nA+B =\n");
    D = sum(A, B);
    printMatrix(out, D);

    fprintf(out, "\nA+A =\n");
    E = sum(A, A);
    printMatrix(out, E);

    fprintf(out, "\nB-A =\n");
    F = diff(B, A);
    printMatrix(out, F);

    fprintf(out, "\nA-A =");
    G = diff(A, A);
    printMatrix(out, G);

    fprintf(out, "\nTranspose(A) =\n");
    H = transpose(A);
    printMatrix(out, H);

    fprintf(out, "\nA*B =\n");
    I = product(A, B);
    printMatrix(out, I);

    fprintf(out, "\nB*B =\n");
    J = product(B, B);
    printMatrix(out, J);


    // free all matrices
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);

    // close all files
    fclose(in);
    fclose(out);
 
    return 0;
}

