# Description

Library for Linear Algebra operations

Example

Create a (2,3) matrix of numbers from 0 to 5

```c
#include "linalg.h"


void main() {

   t_matrix *m = LA_range(6, 2, 3);
   LA_print(m);
   LA_free(m);
}

output:

[[0.000, 1.000, 2.000]
[3.000, 4.000, 5.000]] shape=(2,3)

```



# Types definitions


## 🔧 struct shape


shape of the matrix


```cpp
struct shape
```

**Params:**

- `x` - x dimension
- `y` - y dimension


## 🔧 typedef struct shape t_shape



```cpp
typedef struct shape t_shape
```


## 🔧 struct matrix


the matrix


```cpp
struct matrix
```

**Params:**

- `size` - size of the matrix
- `shape` - shape of the matrix
- `data` - pointer of *double to the matrix data


## 🔧 typedef struct matrix t_matrix



```cpp
typedef struct matrix t_matrix
```


## 🔧 enum ACTIVATION_FNC



```cpp
enum ACTIVATION_FNC
```


## 🔧 struct NN_layer_spec


NN layer specification


```cpp
struct NN_layer_spec
```

**Params:**

- `nunits` - number of Neural Network units
- `activation_function` - activation function of each unit


## 🔧 typedef struct NN_layer_spec t_NN_layer_spec



```cpp
typedef struct NN_layer_spec t_NN_layer_spec
```


## 🔧 struct NN_Layer


the Neural Network layer


```cpp
struct NN_Layer
```

**Params:**

- `W` - weight matrix
- `b` - bias vector
- `dW` - gradient of the loss with respect to W
- `db` - gradient of the loss with respect to b
- `A` - activation of the unit A = f(Z)
- `Z` - Z of the unit (Z = W'X)
- `activation_fnc` - activation function


## 🔧 typedef struct NN_Layer t_NN_layer



```cpp
typedef struct NN_Layer t_NN_layer
```


## 🔧 struct NN_Model


Neural Network model


```cpp
struct NN_Model
```


## 🔹 t_matrix* LA_matrix(int sizey, int sizex)


Create a matrix of (y, x) dimensions.


```cpp
t_matrix* LA_matrix(int sizey, int sizex)
```

**Params:**

- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions

## 🔹 t_matrix* LA_matrixd(const double *data, int sizey, int sizex)


Create a matrix of (y, x) dimensions.


```cpp
t_matrix* LA_matrixd(const double *data, int sizey, int sizex)
```

**Params:**

- `d` - data
- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions

## 🔹 t_matrix* LA_dot(t_matrix *m1, t_matrix *m2)


Execute the dot product between matrix m1 and m2


```cpp
t_matrix* LA_dot(t_matrix *m1, t_matrix *m2)
```

**Params:**

- `m1` - matrix
- `m2` - matrix

**Returns:**

- new matrix with dot product between matrix m1 and m2

## 🔹 t_matrix* LA_dot_atb(t_matrix *A, t_matrix *B)


compute C = (A^T) x B


```cpp
t_matrix* LA_dot_atb(t_matrix *A, t_matrix *B)
```

**Params:**

- `A` - first matrix
- `B` - second matrix

**Returns:**

- (A^T) x B

## 🔹 t_matrix* LA_T(t_matrix *m)


Transpose the matrix


```cpp
t_matrix* LA_T(t_matrix *m)
```

**Params:**

- `m` - matrix to transpose

**Returns:**

- new matrix with transposition of matrix m

## 🔹 t_matrix* LA_slice_rows(t_matrix *m, int start, int end)


Slice matrix rows


```cpp
t_matrix* LA_slice_rows(t_matrix *m, int start, int end)
```

**Params:**

- `m` - matrix
- `start` - start index
- `end` - end index (escxluded)

**Returns:**

- slices of the matrix

## 🔹 t_matrix* LA_slice_cols(t_matrix *m, int start, int end)


Slice matrix cols


```cpp
t_matrix* LA_slice_cols(t_matrix *m, int start, int end)
```

**Params:**

- `m` - matrix
- `start` - start index
- `end` - end index (escxluded)

**Returns:**

- slices of the matrix

## 🔹 t_matrix* LA_slice(t_matrix *m, int start_y, int end_y, int start_x, int end_x)


Slice matrix cols and rows


```cpp
t_matrix* LA_slice(t_matrix *m, int start_y, int end_y, int start_x, int end_x)
```

**Params:**

- `m` - matrix
- `start_y` - start y index
- `end_y` - end y index (escxluded)
- `start_x` - start x index
- `end_x` - end x index (escxluded)

**Returns:**

- slice of the matrix

## 🔹 t_matrix* LA_identity(int n)


Create a square matrix with ones on the main diagonal


```cpp
t_matrix* LA_identity(int n)
```

**Params:**

- `n` - size of the square matrix

**Returns:**

- identity matrix

## 🔹 t_matrix* LA_ones(int sizey, int sizex)


Create a matrix of ones


```cpp
t_matrix* LA_ones(int sizey, int sizex)
```

**Params:**

- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions filled with 1

## 🔹 t_matrix* LA_rand(int sizey, int sizex, int lower, int upper)


Create a matrix of random integer between lower and upper bound


```cpp
t_matrix* LA_rand(int sizey, int sizex, int lower, int upper)
```

**Params:**

- `sizey` - y dimension
- `sizex` - x dimension
- `lower` - lower bound of random integer
- `upper` - upper bound of random integer

**Returns:**

- matrix of (sizey, sizex) dimensions

## 🔹 t_matrix* LA_randf(int sizey, int sizex)


Create a matrix of random float between 0.0 and 1.0


```cpp
t_matrix* LA_randf(int sizey, int sizex)
```

**Params:**

- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex)

## 🔹 t_matrix* LA_sum_rows(t_matrix *m)


Sum matrix rows


```cpp
t_matrix* LA_sum_rows(t_matrix *m)
```

**Params:**

- `m` - matrix

**Returns:**

- matrix of (sizey, 1) dimensions with rows sum

## 🔹 t_matrix* LA_sum_cols(t_matrix *m)


Sum matrix columns


```cpp
t_matrix* LA_sum_cols(t_matrix *m)
```

**Params:**

- `m` - matrix

**Returns:**

- matrix of (1, sizex) dimensions with columns sum

## 🔹 t_matrix* LA_range(int range, int sizey, int sizex)


Create a matrix filled with numbers in range


```cpp
t_matrix* LA_range(int range, int sizey, int sizex)
```

**Params:**

- `range` - range of numbers
- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions

## 🔹 t_matrix* LA_reshape(t_matrix *m, int sizey, int sizex)


resha the matrix with the new sizes.


```cpp
t_matrix* LA_reshape(t_matrix *m, int sizey, int sizex)
```

**Params:**

- `m` - old matrix
- `sizey` - new y dimension
- `sizex` - new x dimension

**Returns:**

- new matrix of (sizey, sizex) dimensions

## 🔹 t_matrix* LA_sumf(t_matrix *m, double f)


sum float to the matrix


```cpp
t_matrix* LA_sumf(t_matrix *m, double f)
```

**Params:**

- `f` - float
- `m` - matrix

**Returns:**

- new matrix

## 🔹 t_matrix* LA_subf(t_matrix *m, double f)


subtract float to the matrix


```cpp
t_matrix* LA_subf(t_matrix *m, double f)
```

**Params:**

- `m` - matrix
- `f` - float

**Returns:**

- new matrix

## 🔹 t_matrix* LA_mulf(t_matrix *m, double f)


multiply the matrix m by scalar f


```cpp
t_matrix* LA_mulf(t_matrix *m, double f)
```

**Params:**

- `m` - matrix
- `f` - float

**Returns:**

- new matrix

## 🔹 t_matrix* LA_divf(t_matrix *m, double f)


dvide the matrix m by scalar f


```cpp
t_matrix* LA_divf(t_matrix *m, double f)
```

**Params:**

- `m` - matrix
- `f` - float

**Returns:**

- new matrix

## 🔹 t_matrix* LA_fsub(double f, t_matrix *m)


subtract float f to matrix m


```cpp
t_matrix* LA_fsub(double f, t_matrix *m)
```

**Params:**

- `f` - float
- `m` - matrix

**Returns:**

- new matrix

## 🔹 t_matrix* LA_sum(t_matrix *m1, t_matrix *m2)


sum matrix m1 with matrix m2


```cpp
t_matrix* LA_sum(t_matrix *m1, t_matrix *m2)
```

**Params:**

- `m1` - matrix
- `m2` - matrix

**Returns:**

- new matrix

## 🔹 t_matrix* LA_sub(t_matrix *m1, t_matrix *m2)


subtract matrix m2 to matrix m1


```cpp
t_matrix* LA_sub(t_matrix *m1, t_matrix *m2)
```

**Params:**

- `m1` - matrix
- `m2` - matrix

**Returns:**

- new matrix

## 🔹 t_matrix* LA_mul(t_matrix *m1, t_matrix *m2)


multiply matrix m1 by matrix m2


```cpp
t_matrix* LA_mul(t_matrix *m1, t_matrix *m2)
```

**Params:**

- `m1` - matrix
- `m2` - matrix

**Returns:**

- new matrix

## 🔹 t_matrix* LA_div(t_matrix *m1, t_matrix *m2)


divide matrix m2 by matrix m1


```cpp
t_matrix* LA_div(t_matrix *m1, t_matrix *m2)
```

**Params:**

- `m1` - matrix
- `m2` - matrix

**Returns:**

- new matrix

## 🔹 int LA_equals(t_matrix *m1, t_matrix *m2)


Compare two matrices


```cpp
int LA_equals(t_matrix *m1, t_matrix *m2)
```

**Params:**

- `m1` - first matrix to compare
- `m2` - second matrix to compare

**Returns:**

- return 1 if m1 equals to m2, 0 otherwise

## 🔹 t_matrix* LA_apply(t_matrix *m, double (*fnc)(double d))


Apply the function to each cell


```cpp
t_matrix* LA_apply(t_matrix *m, double (*fnc)(double d))
```

**Params:**

- `m` - matrix to be modified
- `fnc` - function to be applied

**Returns:**

- nothing

## 🔹 void LA_setval(t_matrix *m, int y, int x, double val)


Set the value at x,y position


```cpp
void LA_setval(t_matrix *m, int y, int x, double val)
```

**Params:**

- `m` - matrix
- `y` - y coordinate
- `x` - x coordinate
- `val` - value of the cell

**Returns:**

- nothing

## 🔹 double LA_getval(t_matrix *m, int y, int x)


Get the value at x,y position


```cpp
double LA_getval(t_matrix *m, int y, int x)
```

**Params:**

- `m` - matrix
- `y` - y coordinate
- `x` - x coordinate

**Returns:**

- cell value

## 🔹 void LA_print(t_matrix *m)


Print the matrix


```cpp
void LA_print(t_matrix *m)
```

**Params:**

- `m` - matrix

**Returns:**

- nothing

## 🔹 void LA_printf(t_matrix *m, char *fmt)


Print the matrix using format fmt


```cpp
void LA_printf(t_matrix *m, char *fmt)
```

**Params:**

- `m` - matrix
- `fmt` - format (es. %.6f)

**Returns:**

- nothing

## 🔹 void LA_nfree(int count, ...)


Destroy n matrices


```cpp
void LA_nfree(int count, ...)
```

**Params:**

- `count` - number of matrices passed as arguments
- `...` - m1, m2, ..., mn matrices to be destroied

**Returns:**

- nothing

## 🔹 void LA_free(t_matrix *m)


Destroy the matrix


```cpp
void LA_free(t_matrix *m)
```

**Params:**

- `m` - matrix to be destroied

**Returns:**

- nothing

## 🔹 double fn_sigmoid(double x)


sigmoid function


```cpp
double fn_sigmoid(double x)
```

**Params:**

- `x` - x value

**Returns:**

- sigmoid of x

## 🔹 double fn_dsigmoid(double x)


sigmoid derivative function


```cpp
double fn_dsigmoid(double x)
```

**Params:**

- `x` - x value

**Returns:**

- sigmoid derivative of x

## 🔹 double fn_negative(double x)


negative function


```cpp
double fn_negative(double x)
```

**Params:**

- `x` - x value

**Returns:**

- -x

## 🔹 double fn_relu(double x)


ReLU function


```cpp
double fn_relu(double x)
```


## 🔹 double fn_drelu(double x)


the ReLU derivative function


```cpp
double fn_drelu(double x)
```


## 🔹 t_NN_model *NN_create_model(int ninputs, int nlayers, struct NN_layer_spec spec[])


Create the Neural Network Model using NN_layer_spec



```cpp
t_NN_model *NN_create_model(int ninputs, int nlayers, struct NN_layer_spec spec[])
```

**Params:**

- `ninputs` - number of inputs of the model
- `nlayers` - number of layers
- `spec` - layers specification


## 🔹 t_NN_layer *NN_create_layer(int nunits, int ninputs, enum ACTIVATION_FNC fnc)


Create the Neural Network Layer


```cpp
t_NN_layer *NN_create_layer(int nunits, int ninputs, enum ACTIVATION_FNC fnc)
```

**Params:**

- `nunits` - number of units of the layer
- `ninputs` - number of inputs of each unit (same as nunits of previous layer)
- `fnc` - activation function

**Returns:**

- the Neural Network Layer

## 🔹 void NN_print_model(t_NN_model *model)


print the Neural Network Model


```cpp
void NN_print_model(t_NN_model *model)
```

**Params:**

- `model` - the model
- `fmt` - format of matrix cells (es. %.6f)


## 🔹 void NN_free_model(t_NN_model *model)


free the model



```cpp
void NN_free_model(t_NN_model *model)
```

**Params:**

- `model` - the model


## 🔹 void NN_free_layer(t_NN_layer *layer)


free the layer


```cpp
void NN_free_layer(t_NN_layer *layer)
```

**Params:**

- `layer` - the layer


## 🔹 void NN_model_execute(t_matrix *input, t_NN_model *model)


Execute the model, compute Z and A (activation)



```cpp
void NN_model_execute(t_matrix *input, t_NN_model *model)
```

**Params:**

- `input` - input matrix
- `model` - to execute

