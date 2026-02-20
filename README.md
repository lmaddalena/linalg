# Description

Library for Linear Algebra operations

## Example
    
    Create a (2,3) matrix of numbers from 0 to 5
    
```c
#include "linalg.h"


void main() {

   t_LinAlg la = LinAlg_Init();
   t_matrix m = la.range(6, 2, 3);
   la.print(m);
}

output:

[[0.000, 1.000, 2.000]
[3.000, 4.000, 5.000]] shape=(2,3)

```
    

    
# Types definitions


## 🔧 struct shape


#### shape of the matrix


```cpp
struct shape
```

**Params:**

- `x` - x dimension
- `y` - y dimension


## 🔧 typedef struct shape t_shape

t_shape

```cpp
typedef struct shape t_shape
```


## 🔧 struct matrix


#### the matrix


```cpp
struct matrix
```

**Params:**

- `size` - size of the matrix
- `shape` - shape of the matrix
- `data` - pointer of *double to the matrix data


## 🔧 typedef struct matrix t_matrix

t_matrix

```cpp
typedef struct matrix t_matrix
```


## 🔧 struct LinAlg


#### Linear Algebra Api's

```cpp
struct LinAlg
```


## 🔹 t_matrix (*matrix)(int sizey, int sizex)


#### Create a matrix of (y, x) dimensions.


```cpp
t_matrix (*matrix)(int sizey, int sizex)
```

**Params:**

- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions

## 🔹 t_matrix (*matrixd)(double *data, int sizey, int sizex)


#### Create a matrix of (y, x) dimensions.


```cpp
t_matrix (*matrixd)(double *data, int sizey, int sizex)
```

**Params:**

- `d` - data
- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions

## 🔹 t_matrix (*dot)(t_matrix m1, t_matrix m2)


#### Execute the dot product between matrix m1 and m2


```cpp
t_matrix (*dot)(t_matrix m1, t_matrix m2)
```

**Params:**

- `m1` - matrix
- `m2` - matrix

**Returns:**

- new matrix with dot product between matrix m1 and m2

## 🔹 t_matrix (*T)(t_matrix m)


#### Transpose the matrix


```cpp
t_matrix (*T)(t_matrix m)
```

**Params:**

- `m` - matrix to transpose

**Returns:**

- new matrix with transposition of matrix m

## 🔹 void (*frees)(int count, ...)


#### Destroy n matrices


```cpp
void (*frees)(int count, ...)
```

**Params:**

- `count` - number of matrices passed as arguments
- `...` - m1, m2, ..., mn matrices to be destroied

**Returns:**

- nothing

## 🔹 void (*free)(t_matrix m)


#### Destroy the matrix


```cpp
void (*free)(t_matrix m)
```

**Params:**

- `m` - matrix to be destroied

**Returns:**

- nothing

## 🔹 double (*getval)(t_matrix m, int y, int x)


#### Get the value at x,y position


```cpp
double (*getval)(t_matrix m, int y, int x)
```

**Params:**

- `m` - matrix
- `y` - y coordinate
- `x` - x coordinate

**Returns:**

- cell value

## 🔹 void (*setval)(t_matrix m, int y, int x, double val)


#### Set the value at x,y position


```cpp
void (*setval)(t_matrix m, int y, int x, double val)
```

**Params:**

- `m` - matrix
- `y` - y coordinate
- `x` - x coordinate
- `val` - value of the cell

**Returns:**

- nothing

## 🔹 void (*print)(t_matrix m)


#### Print the matrix


```cpp
void (*print)(t_matrix m)
```

**Params:**

- `m` - matrix

**Returns:**

- nothing

## 🔹 t_matrix (*slice_rows)(t_matrix m, int start, int end)


#### Slice matrix rows


```cpp
t_matrix (*slice_rows)(t_matrix m, int start, int end)
```

**Params:**

- `m` - matrix
- `start` - start index
- `end` - end index (escxluded)

**Returns:**

- slices of the matrix

## 🔹 t_matrix (*slice_cols)(t_matrix m, int start, int end)


#### Slice matrix rows


```cpp
t_matrix (*slice_cols)(t_matrix m, int start, int end)
```

**Params:**

- `m` - matrix
- `start` - start index
- `end` - end index (escxluded)

**Returns:**

- slices of the matrix

## 🔹 void (*apply)(t_matrix m, double (*fnc)(double d))


#### Apply the function to each cell


```cpp
void (*apply)(t_matrix m, double (*fnc)(double d))
```

**Params:**

- `m` - matrix to be modified
- `fnc` - function to be applied

**Returns:**

- nothing

## 🔹 t_matrix (*identity)(int n)


#### Create a square matrix with ones on the main diagonal


```cpp
t_matrix (*identity)(int n)
```

**Params:**

- `n` - size of the square matrix

**Returns:**

- identity matrix

## 🔹 t_matrix (*ones)(int sizex, int sizey)


#### Create a matrix of ones


```cpp
t_matrix (*ones)(int sizex, int sizey)
```

**Params:**

- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions filled with 1

## 🔹 t_matrix (*rand)(int sizex, int sizey, int lower, int upper)


#### Create a matrix of random integer between lower and upper bound


```cpp
t_matrix (*rand)(int sizex, int sizey, int lower, int upper)
```

**Params:**

- `sizey` - y dimension
- `sizex` - x dimension
- `lower` - lower bound of random integer
- `upper` - upper bound of random integer

**Returns:**

- matrix of (sizey, sizex) dimensions filled with 1

## 🔹 t_matrix (*sum_rows)(t_matrix m)


#### Sum matrix rows


```cpp
t_matrix (*sum_rows)(t_matrix m)
```

**Params:**

- `m` - matrix

**Returns:**

- matrix of (sizey, 1) dimensions with rows sum

## 🔹 t_matrix (*sum_cols)(t_matrix m)


#### Sum matrix columns


```cpp
t_matrix (*sum_cols)(t_matrix m)
```

**Params:**

- `m` - matrix

**Returns:**

- matrix of (1, sizex) dimensions with columns sum

## 🔹 int (*equals)(t_matrix m1, t_matrix m2)


#### Compare two matrices


```cpp
int (*equals)(t_matrix m1, t_matrix m2)
```

**Params:**

- `m1` - first matrix to compare
- `m2` - second matrix to compare

**Returns:**

- return 1 if m1 equals m2, 0 otherwise

## 🔹 t_matrix (*range)(int range, int sizey, int sizex)


#### Create a matrix filled with numbers in range


```cpp
t_matrix (*range)(int range, int sizey, int sizex)
```

**Params:**

- `range` - range of numbers
- `sizey` - y dimension
- `sizex` - x dimension

**Returns:**

- matrix of (sizey, sizex) dimensions

## 🔹 t_matrix (*reshape)(t_matrix m, int sizey, int sizex)


#### resha the matrix with the new sizes.


```cpp
t_matrix (*reshape)(t_matrix m, int sizey, int sizex)
```

**Params:**

- `m` - old matrix
- `sizey` - new y dimension
- `sizex` - new x dimension

**Returns:**

- new matrix of (sizey, sizex) dimensions

## 🔧 typedef struct LinAlg t_LinAlg

t_LinAlg

```cpp
typedef struct LinAlg t_LinAlg
```


# Functions

## 🔹 t_LinAlg LinAlg_Init()


#### Initialize LinAlg


```cpp
t_LinAlg LinAlg_Init()
```

**Returns:**

- t_LinAlg type
