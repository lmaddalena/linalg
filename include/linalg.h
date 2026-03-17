#ifndef LINALG_H
#define LINALG_H

/**
 * # Description
 * 
 * Library for Linear Algebra operations
 *
 * Example
 *
 * Create a (2,3) matrix of numbers from 0 to 5
 *
 * ```c
 * #include "linalg.h"
 *
 *
 * void main() {
 * 
 *    t_matrix *m = LA_range(6, 2, 3);
 *    LA_print(m);
 *    LA_free(m);
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
  * shape of the matrix
  * 
  * @param x x dimension
  * @param y y dimension
  */
struct shape
{
    int x;
    int y;
};

/** t_shape
 * 
 */
typedef struct shape t_shape;

/** matrix
 * 
 * the matrix
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

/** t_matrix
 * 
 */
typedef struct matrix t_matrix;

/**  ACTIVATION_FNC
 * 
 */
enum ACTIVATION_FNC
{
    SIGMOID,
    RELU
};

/** NN_layer_spec
 * 
 * NN layer specification
 * 
 * @param nunits number of Neural Network units
 * @param activation_function activation function of each unit
 */
struct NN_layer_spec
{
    int nunits;
    enum ACTIVATION_FNC activation_function;
};

/** t_NN_layer_spec
 * 
 */
typedef struct NN_layer_spec t_NN_layer_spec;



/** NN_layer
 * 
 * the Neural Network layer
 * 
 * @param W weight matrix
 * @param b bias vector
 * @param dW gradient of the loss with respect to W
 * @param db gradient of the loss with respect to b
 * @param A activation of the unit A = f(Z)
 * @param Z Z of the unit (Z = W'X)
 * @param activation_fnc activation function
 */
struct NN_Layer
{
    t_matrix *W;
    t_matrix *b;
    t_matrix *dW;
    t_matrix *db;
    t_matrix *A;
    t_matrix *Z;

    enum ACTIVATION_FNC activation_fnc;
};

/** t_NN_layer
 * 
 */
typedef struct NN_Layer t_NN_layer;

/** NN_Model
 * 
 * Neural Network model
 * 
 */
struct NN_Model
{
    t_NN_layer **layers;
    int nlayers;    
};

typedef struct NN_Model t_NN_model;

/** LA_matrix
*
* Create a matrix of (y, x) dimensions.
*
* @param sizey y dimension
* @param sizex x dimension
* @return matrix of (sizey, sizex) dimensions
*/        
t_matrix* LA_matrix(int sizey, int sizex);

/** LA_matrixd
*
* Create a matrix of (y, x) dimensions.
*
* @param d data 
* @param sizey y dimension
* @param sizex x dimension
* @return matrix of (sizey, sizex) dimensions
*/        
t_matrix* LA_matrixd(const double *data, int sizey, int sizex);

/** LA_dot
*
* Execute the dot product between matrix m1 and m2
*
* @param m1 matrix
* @param m2 matrix
* @return new matrix with dot product between matrix m1 and m2
*/        
t_matrix* LA_dot(t_matrix *m1, t_matrix *m2);

/**  LA_dot_atb
*
* compute C = (A^T) x B
* 
* @param A first matrix
* @param B second matrix
* @return (A^T) x B
*/ 
t_matrix* LA_dot_atb(t_matrix *A, t_matrix *B);

/** T
*
* Transpose the matrix
*
* @param m matrix to transpose
* @return new matrix with transposition of matrix m
*/        
t_matrix* LA_T(t_matrix *m);

/** LA_slice_rows
* 
* Slice matrix rows
*
* @param m matrix
* @param start start index  
* @param end end index (escxluded)
* @return slices of the matrix
*/        
t_matrix* LA_slice_rows(t_matrix *m, int start, int end);

/** LA_slice_cols
* 
* Slice matrix cols
*
* @param m matrix
* @param start start index  
* @param end end index (escxluded)
* @return slices of the matrix
*/        
t_matrix* LA_slice_cols(t_matrix *m, int start, int end);

/** LA_slice
* 
* Slice matrix cols and rows
*
* @param m matrix
* @param start_y start y index
* @param end_y end y index (escxluded)
* @param start_x start x index
* @param end_x end x index (escxluded)
* @return slice of the matrix
*/        
t_matrix* LA_slice(t_matrix *m, int start_y, int end_y, int start_x, int end_x);

/** LA_identity
*
* Create a square matrix with ones on the main diagonal
* 
* @param n size of the square matrix
* @return identity matrix
*/        
t_matrix* LA_identity(int n);

/** LA_ones
* 
* Create a matrix of ones
*
* @param sizey y dimension
* @param sizex x dimension
* @return matrix of (sizey, sizex) dimensions filled with 1
*/        
t_matrix* LA_ones(int sizey, int sizex);

/** LA_rand
*
* Create a matrix of random integer between lower and upper bound
* 
* @param sizey y dimension
* @param sizex x dimension
* @param lower lower bound of random integer
* @param upper upper bound of random integer
* @return matrix of (sizey, sizex) dimensions
*/        
t_matrix* LA_rand(int sizey, int sizex, int lower, int upper);

/** LA_randf
*
* Create a matrix of random float between 0.0 and 1.0
* 
* @param sizey y dimension
* @param sizex x dimension
* @return matrix of (sizey, sizex)
*/        
t_matrix* LA_randf(int sizey, int sizex);

/** LA_sum_rows
* 
* Sum matrix rows
*
* @param m matrix
* @return matrix of (sizey, 1) dimensions with rows sum
*/        
t_matrix* LA_sum_rows(t_matrix *m);

/** LA_sum_cols
* 
* Sum matrix columns
*
* @param m matrix
* @return matrix of (1, sizex) dimensions with columns sum
*/        
t_matrix* LA_sum_cols(t_matrix *m);

/** LA_range
 * 
 * Create a matrix filled with numbers in range
*
* @param range range of numbers
* @param sizey y dimension
* @param sizex x dimension
* @return matrix of (sizey, sizex) dimensions
*/        
t_matrix* LA_range(int range, int sizey, int sizex);

/** LA_reshape
*
* resha the matrix with the new sizes.
*
* @param m old matrix
* @param sizey new y dimension
* @param sizex new x dimension
* @return new matrix of (sizey, sizex) dimensions
*/        
t_matrix* LA_reshape(t_matrix *m, int sizey, int sizex);

/** LA_sumf
 * 
 * sum float to the matrix
 * 
 * @param f float
 * @param m matrix
 * @return new matrix 
 */
t_matrix* LA_sumf(t_matrix *m, double f);

/** LA_subf
 * 
 * subtract float to the matrix
 * 
 * @param m matrix
 * @param f float
 * @return new matrix 
 */
t_matrix* LA_subf(t_matrix *m, double f);

/** LA_mulf
 * 
 * multiply the matrix m by scalar f
 * 
 * @param m matrix
 * @param f float
 * @return new matrix 
 */
t_matrix* LA_mulf(t_matrix *m, double f);

/** LA_divf
 * 
 * dvide the matrix m by scalar f
 * 
 * @param m matrix
 * @param f float
 * @return new matrix 
 */
t_matrix* LA_divf(t_matrix *m, double f);

/** LA_fsub
 * 
 * subtract float f to matrix m
 * 
 * @param f float
 * @param m matrix
 * @return new matrix 
 */
t_matrix* LA_fsub(double f, t_matrix *m);

/** LA_sum
 * 
 * sum matrix m1 with matrix m2
 * 
 * @param m1 matrix
 * @param m2 matrix
 * @return new matrix 
 */
t_matrix* LA_sum(t_matrix *m1, t_matrix *m2);

/** LA_sub
 * 
 * subtract matrix m2 to matrix m1
 * 
 * @param m1 matrix
 * @param m2 matrix
 * @return new matrix 
 */
t_matrix* LA_sub(t_matrix *m1, t_matrix *m2);

/** LA_mul
 * 
 * multiply matrix m1 by matrix m2
 * 
 * @param m1 matrix
 * @param m2 matrix
 * @return new matrix 
 */
t_matrix* LA_mul(t_matrix *m1, t_matrix *m2);

/** LA_div
 * 
 * divide matrix m2 by matrix m1
 * 
 * @param m1 matrix
 * @param m2 matrix
 * @return new matrix 
 */
t_matrix* LA_div(t_matrix *m1, t_matrix *m2);

/** LA_equals
* 
* Compare two matrices
*
* @param m1 first matrix to compare
* @param m2 second matrix to compare
* @return return 1 if m1 equals to m2, 0 otherwise
*/        
int LA_equals(t_matrix *m1, t_matrix *m2);

/** LA_apply
* 
* Apply the function to each cell
*
* @param m matrix to be modified
* @param fnc function to be applied
* @return nothing
*/        
t_matrix* LA_apply(t_matrix *m, double (*fnc)(double d));

/** LA_setval
*
* Set the value at x,y position
*
* @param m matrix
* @param y y coordinate
* @param x x coordinate
* @param val value of the cell
* @return nothing
*/        
void LA_setval(t_matrix *m, int y, int x, double val);

/** LA_getval
*
* Get the value at x,y position
*
* @param m matrix
* @param y y coordinate
* @param x x coordinate
* @return cell value
*/        
double LA_getval(t_matrix *m, int y, int x);

/** LA_print
* 
* Print the matrix
*
* @param m matrix
* @return nothing
*/        
void LA_print(t_matrix *m);

/** LA_printf
* 
* Print the matrix using format fmt
*
* @param m matrix
* @param fmt format (es. %.6f)
* @return nothing
*/        
void LA_printf(t_matrix *m, char *fmt);

/** LA_nfree
*
* Destroy n matrices
* 
* @param count number of matrices passed as arguments
* @param ... m1, m2, ..., mn matrices to be destroied
* @return nothing
*/        
void LA_nfree(int count, ...);

/** LA_free
*
* Destroy the matrix
*
* @param m matrix to be destroied
* @return nothing
*/        
void LA_free(t_matrix *m);

/** fn_sigmoid
 * 
 * sigmoid function
 * 
 * @param x x value
 * @return sigmoid of x
 */
double fn_sigmoid(double x);

/** fn_dsigmoid
 * 
 * sigmoid derivative function
 * 
 * @param x x value
 * @return sigmoid derivative of x
 */
double fn_dsigmoid(double x);

/** fn_negative
 * 
 * negative function
 * 
 * @param x x value
 * @return -x
 */
double fn_negative(double x);

/** fn_relu
 * 
 * ReLU function
 * 
 */
double fn_relu(double x);

/** fn_drelu
 * 
 * the ReLU derivative function
 * 
 */
double fn_drelu(double x);

/** NN_create_model
 * 
 * Create the Neural Network Model using NN_layer_spec
 * 
 * @param ninputs number of inputs of the model
 * @param nlayers number of layers
 * @param spec layers specification
 * 
 */
t_NN_model *NN_create_model(int ninputs, int nlayers, struct NN_layer_spec spec[]);

/** NN_create_layer
 * 
 * Create the Neural Network Layer
 * 
 * @param nunits number of units of the layer
 * @param ninputs number of inputs of each unit (same as nunits of previous layer)
 * @param fnc activation function
 * @return the Neural Network Layer
 */
t_NN_layer *NN_create_layer(int nunits, int ninputs, enum ACTIVATION_FNC fnc);

/** NN_print_model
 * 
 * print the Neural Network Model
 * 
 * @param model the model
 * @param fmt format of matrix cells (es. %.6f)
 */
void NN_print_model(t_NN_model *model);

/** NN_free_model
 * 
 * free the model
 * 
 * @param model the model
 * 
 */
void NN_free_model(t_NN_model *model);

/** NN_free_layer
 * 
 * free the layer
 * 
 * @param layer the layer
 */
void NN_free_layer(t_NN_layer *layer);

/** NN_model_execute
 * 
 * Execute the model, compute Z and A (activation)
 * 
 * @param input input matrix
 * @param model to execute
 * 
 */
void NN_model_execute(t_matrix *input, t_NN_model *model);

#endif

