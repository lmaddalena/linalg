#ifndef LINALG_H
#define LINALG_H

/**
 * # Description
 * 
 * Library for Linear Algebra operations
 *
 * ## Example
 *
 * Create a (2,3) matrix of numbers from 0 to 5
 *
 * ```c
 * #include "linalg.h"
 *
 *
 * void main() {
 * 
 *    t_LinAlg la = LinAlg_Init();
 *    t_matrix m = la.range(6, 2, 3);
 *    la.print(m);
 * }
 *
 * output:
 * 
 * [[0.000, 1.000, 2.000]
 * [3.000, 4.000, 5.000]] shape=(2,3)
 * 
 * ```
 *
 */

/** 
 * 
 * # Types definitions
 * 
 */

 /** shape
  * 
  * ## shape of the matrix
  * 
  * @param x x dimension
  * @param y y dimension
  */
struct shape
{
    int x;
    int y;
};

/**
 * t_shape
 */
typedef struct shape t_shape;

/** matrix
 * 
 * ## the matrix
 * 
 * @param size size of the matrix
 * @param shape shape of the matrix
 * @param data pointer of *double to the matrix data
 */
struct matrix
{
    int size;
    t_shape shape;      
    double * data;        
};

/**
 * t_matrix
 */
typedef struct matrix t_matrix;


/** LinAlg
 * 
 * ## Linear Algebra Api's
*/
struct LinAlg
{
    /** matrix
    *
    * ## Create a matrix of (y, x) dimensions.
    *
    * @param sizey y dimension
    * @param sizex x dimension
    * @return matrix of (sizey, sizex) dimensions
    */        
    t_matrix (*matrix)(int sizey, int sizex);

    /** matrix
    *
    * ## Create a matrix of (y, x, z) dimensions.
    *
    * @param sizey y dimension
    * @param sizex x dimension
    * @param sizez z dimension
    * @return matrix of (sizey, sizex, sizez) dimensions
    */        
    t_matrix * (*matrix3d)(int sizey, int sizex, int sizez);

    /** matrix3d
    *
    * ## Create a matrix of (y, x) dimensions.
    *
    * @param d data 
    * @param sizey y dimension
    * @param sizex x dimension
    * @return matrix of (sizey, sizex) dimensions
    */        
    t_matrix (*matrixd)(double *data, int sizey, int sizex);

    /** Dot
    *
    * ## Execute the dot product between matrix m1 and m2
    *
    * @param m1 matrix
    * @param m2 matrix
    * @return new matrix with dot product between matrix m1 and m2
    */        
    t_matrix (*dot)(t_matrix m1, t_matrix m2);

    /** T
    *
    * ## Transpose the matrix
    *
    * @param m matrix to transpose
    * @return new matrix with transposition of matrix m
    */        
    t_matrix (*T)(t_matrix m);

    /** frees
    *
    * ## Destroy n matrices
    * 
    * @param count number of matrices passed as arguments
    * @param ... m1, m2, ..., mn matrices to be destroied
    * @return nothing
    */        
    void (*frees)(int count, ...);

    /** free
    *
    * ## Destroy the matrix
    *
    * @param m matrix to be destroied
    * @return nothing
    */        
    void (*free)(t_matrix m);

    /** getval
    *
    * ## Get the value at x,y position
    *
    * @param m matrix
    * @param y y coordinate
    * @param x x coordinate
    * @return cell value
    */        
    double (*getval)(t_matrix m, int y, int x); 

   /** setval
    *
    * ## Set the value at x,y position
    *
    * @param m matrix
    * @param y y coordinate
    * @param x x coordinate
    * @param val value of the cell
    * @return nothing
    */        
    void (*setval)(t_matrix m, int y, int x, double val);

   /** print
    * 
    * ## Print the matrix
    *
    * @param m matrix
    * @return nothing
    */        
    void (*print)(t_matrix m);

   /** slice_rows
    * 
    * ## Slice matrix rows
    *
    * @param m matrix
    * @param start start index  
    * @param end end index (escxluded)
    * @return slices of the matrix
    */        
    t_matrix (*slice_rows)(t_matrix m, int start, int end);

   /** slice_cols
    * 
    * ## Slice matrix rows
    *
    * @param m matrix
    * @param start start index  
    * @param end end index (escxluded)
    * @return slices of the matrix
    */        
    t_matrix (*slice_cols)(t_matrix m, int start, int end);

   /** apply
    * 
    * ## Apply the function to each cell
    *
    * @param m matrix to be modified
    * @param fnc function to be applied
    * @return nothing
    */        
    void (*apply)(t_matrix m, double (*fnc)(double d));

   /** identity
    *
    * ## Create a square matrix with ones on the main diagonal
    * 
    * @param n size of the square matrix
    * @return identity matrix
    */        
    t_matrix (*identity)(int n);

   /** ones
    * 
    * ## Create a matrix of ones
    *
    * @param sizey y dimension
    * @param sizex x dimension
    * @return matrix of (sizey, sizex) dimensions filled with 1
    */        
    t_matrix (*ones)(int sizex, int sizey);

   /** rand
    *
    * ## Create a matrix of random integer between lower and upper bound
    * 
    * @param sizey y dimension
    * @param sizex x dimension
    * @param lower lower bound of random integer
    * @param upper upper bound of random integer
    * @return matrix of (sizey, sizex) dimensions filled with 1
    */        
    t_matrix (*rand)(int sizex, int sizey, int lower, int upper);

   /** sum_rows
    * 
    * ## Sum matrix rows
    *
    * @param m matrix
    * @return matrix of (sizey, 1) dimensions with rows sum
    */        
    t_matrix (*sum_rows)(t_matrix m);

   /** sum_cols
    * 
    * ## Sum matrix columns
    *
    * @param m matrix
    * @return matrix of (1, sizex) dimensions with columns sum
    */        
    t_matrix (*sum_cols)(t_matrix m);

   /** compare
    * 
    * ## Compare two matrices
    *
    * @param m1 first matrix to compare
    * @param m2 second matrix to compare
    * @return return 1 if m1 equals m2, 0 otherwise
    */        
    int (*equals)(t_matrix m1, t_matrix m2);

    /** range
     * 
     * ## Create a matrix filled with numbers in range
    *
    * @param range range of numbers
    * @param sizey y dimension
    * @param sizex x dimension
    * @return matrix of (sizey, sizex) dimensions
    */        
    t_matrix (*range)(int range, int sizey, int sizex);

};

/**
 * t_LinAlg
 */
typedef struct LinAlg t_LinAlg;

/**
 * # Functions
 */

/** LinAlg_Init()
*
* ## Initialize LinAlg
* 
* @return t_LinAlg type
*/        
t_LinAlg LinAlg_Init();

    
#endif