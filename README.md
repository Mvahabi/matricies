# Assignment pa4 : Matrix ADTs 

In this assignment you will create a calculator for performing matrix
operations. This project will utilize 
a List ADT (List.h and List.c), in order to perform matrix operations.
The main program for this project is called Sparse.c. An 𝑛 × 𝑛 square
matrix is said to be sparse if the number of non-zero
entries (here abbreviated NNZ) is small compared to the total number of
entries 𝑛^2. The result will be a C program capable of performing fast
matrix operations, even on very large matrices, provided they are
sparse.
Matrix.h and Matrix.c, and will export its services to the client module
MatrixTest.c and Sparse.c.

## Build
    
    $ make

## Running

### ./Sparse "in1" "out1"
### ./MatrixTest

./Sparse will take two command line arguments
giving the names of the input and output files, respectively. The input file
will begin with a single line
containing three integers n, a and b, separated by spaces. The second line
will be blank, and the following
a lines will specify the non-zero entries of an 𝑛 × 𝑛 matrix A. Each of
these lines will contain a space
separated list of three numbers: two integers and a double, giving the row,
column, and value of the
corresponding matrix entry. After another blank line, will follow b lines
specifying the non-zero entries of
an 𝑛 × 𝑛 matrix B. For example, the two matrices

No need to do anything for ./MatrixTest
