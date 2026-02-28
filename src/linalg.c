#include "../include/linalg.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <math.h>

/* ========================== MACROS ============================ */
#define ASSERT(exp, msg) \
        do { \
            if (!(exp)) { \
                fprintf(stderr, "ASSERT FAILED: %s\nFile: %s, Line: %d\nMessage: %s\n\n", \
                        #exp, __FILE__, __LINE__, msg); \
                abort(); \
            } \
        } while (0)


/* ========================== PROTOTYPES ============================ */

static t_matrix* matrix_create(int sizey, int sizex);
static t_matrix* matrix_create_d(const double *data, int sizey, int sizex);
static void assert_index(t_matrix *m, int y, int x);
static void matrix_setval(t_matrix *m, int y, int x, double val);
static double matrix_getval(t_matrix *m, int y, int x);
static void matrix_print_row(t_matrix *m, int y);
static void matrix_print(t_matrix *m);
static void matrix_frees(int count, ...);
static void matrix_free(t_matrix *m);
static t_matrix* matrix_dot(t_matrix *m1, t_matrix *m2);
static t_matrix* matrix_matmul_atb(t_matrix *A, t_matrix *B);
static t_matrix* matrix_T(t_matrix *m);
static t_matrix* matrix_slice_rows(t_matrix *m, int start, int end);
static t_matrix* matrix_slice_cols(t_matrix *m, int start, int end);
static t_matrix* matrix_slice(t_matrix *m, int start_y, int end_y, int start_x, int end_x);
static void matrix_apply(t_matrix *m, double (*fnc)(double d));
static t_matrix* matrix_identity(int n);
static t_matrix* matrix_ones(int sizey, int sizex);
static t_matrix* matrix_rand(int sizey, int sizex, int lower, int upper);
static t_matrix* matrix_sum_rows(t_matrix *m);
static t_matrix* matrix_sum_cols(t_matrix *m);
static int matrix_equals(t_matrix *m1, t_matrix *m2);
static t_matrix* matrix_range(int range, int sizey, int sizex);
static t_matrix* matrix_reshape(t_matrix *m, int sizey, int sizex);
static double fn_sigmoid(double x);
static double fn_dsigmoid(double x);
static double fn_negative(double x);
static t_matrix* matrix_sumf(t_matrix *m, double f);
static t_matrix* matrix_subf(t_matrix *m, double f);
static t_matrix* matrix_mulf(t_matrix *m, double f);
static t_matrix* matrix_divf(t_matrix *m, double f);
static t_matrix* matrix_fsub(double f, t_matrix *m);
static t_matrix* matrix_sum(t_matrix *m1, t_matrix *m2);
static t_matrix* matrix_sub(t_matrix *m1, t_matrix *m2);
static t_matrix* matrix_mul(t_matrix *m1, t_matrix *m2);
static t_matrix* matrix_div(t_matrix *m1, t_matrix *m2);

/* ========================== API ============================ */

/* Initialize Linear Algebra
*
* @return the t_LinAlg structure to gain access to the linear algebra functions
*/        
t_LinAlg LinAlg_Init()
{
    t_LinAlg la;
    la.matrixd = matrix_create_d;
    la.matrix = matrix_create;
    la.dot = matrix_dot;
    la.matmul_atb = matrix_matmul_atb;
    la.frees = matrix_frees;
    la.free = matrix_free;
    la.getval = matrix_getval;
    la.setval = matrix_setval;
    la.print = matrix_print;
    la.T = matrix_T;
    la.slice = matrix_slice;
    la.slice_rows = matrix_slice_rows;
    la.slice_cols = matrix_slice_cols;
    la.apply = matrix_apply;
    la.identity = matrix_identity;
    la.ones = matrix_ones;
    la.rand = matrix_rand;
    la.sum_rows = matrix_sum_rows;
    la.sum_cols = matrix_sum_cols;
    la.equals = matrix_equals;
    la.range = matrix_range;
    la.reshape = matrix_reshape;
    la.fn_sigmoid = fn_sigmoid;
    la.fn_dsigmoid = fn_dsigmoid;
    la.fn_negative = fn_negative;
    la.sumf = matrix_sumf;
    la.subf = matrix_subf;
    la.fsub = matrix_fsub;
    la.sum = matrix_sum;
    la.sub = matrix_sub;
    la.mul = matrix_mul;
    la.div = matrix_div;
    la.mulf = matrix_mulf;
    la.divf = matrix_divf;

    return la;
}


/* ========================== STATIC FUNCTIONS ============================ */

static t_matrix* matrix_rand(int sizey, int sizex, int lower, int upper)
{
    t_matrix *m = matrix_create(sizey, sizex);

    srand(time(NULL));

    for(int i = 0; i < m->size; i++)
        m->data[i] =  (rand() % upper) + lower;

    return m;    
}

static t_matrix* matrix_ones(int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    t_matrix* m = matrix_create(sizey, sizex);

    for(int i = 0; i < m->size; i++)
        m->data[i] = 1;

    return m;    
}

static t_matrix* matrix_identity(int n)
{
    t_matrix *m = matrix_create(n, n);

    for(int y = 0; y < n; y ++)
        for(int x = 0; x < n; x++)
            matrix_setval(m, y, x, y == x ? 1 : 0);

    return m;
}

static void matrix_apply(t_matrix *m, double (*fnc)(double d))
{
    for(int i = 0; i < m->size; i++)
        m->data[i] = fnc(m->data[i]);
}

static t_matrix* matrix_create(int sizey, int sizex)
{
    t_matrix *m;

    m = malloc(sizeof(t_matrix));
    if (m == NULL) {
        ASSERT(0, "out of memory for matrix struct");
        return NULL;
    }


    m->shape.x = sizex;
    m->shape.y = sizey;
    m->size = sizex * sizey;
    m->data = calloc(m->size, sizeof(double));

    if (m->data == NULL) {
        free(m);
        ASSERT(0, "out of memory for matrix data");
        return NULL;
    }


    return m;    
}

static t_matrix* matrix_create_d(const double *data, int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");
    ASSERT(data != NULL, "input data pointer is null");

    t_matrix *m = malloc(sizeof(t_matrix));
    if (m == NULL) {
        ASSERT(0, "out of memory for matrix struct");
        return NULL;
    }

    m->shape.x = sizex;
    m->shape.y = sizey;
    m->size = sizex * sizey;
    m->data = malloc(sizeof(double) * m->size);

    if (m->data == NULL) {
        free(m);
        ASSERT(0, "out of memory for matrix data");
        return NULL;
    }

    memcpy(m->data, data, m->size * sizeof(double));

    return m;    
}

static void assert_index(t_matrix *m, int y, int x)
{
    ASSERT(y >= 0 && y < m->shape.y, "y is out of bound");
    ASSERT(x >= 0 && x < m->shape.x, "x is out of bound");
}

static void matrix_setval(t_matrix *m, int y, int x, double val) 
{
    assert_index(m, y, x);
    m->data[y * m->shape.x + x] = val;
}

static double matrix_getval(t_matrix *m, int y, int x) 
{
    assert_index(m, y, x);
    return m->data[y * m->shape.x + x];
}

static void matrix_print_row(t_matrix *m, int y)
{
    int maxcount = 10;
    int max = m->shape.x;

    if(m->shape.x > maxcount)
        max = maxcount / 2;        

    char format[] = "%.3f";
    
    printf("[");
    for(int x = 0; x < max; x++)
    {
        printf(format, matrix_getval(m, y, x));
        if(x < m->shape.x -1)
            printf(", ");

    }

    if(max < m->shape.x)
    {
        printf("..., ");
        for(int x = m->shape.x - max; x < m->shape.x; x++)
        {                   
            printf("%.3f", matrix_getval(m, y, x));
            if(x < m->shape.x -1)
                printf(", ");
        }

    }
    printf("]");
    if(y < m->shape.y -1)
        printf("\n");
}

static void matrix_print(t_matrix *m)
{
    int maxcount = 10;
    int max = m->shape.y;

    if(m->shape.y > maxcount)
        max = maxcount / 2; 

    printf("[");
    for(int y = 0; y < max; y++)
    {
        matrix_print_row(m, y);
    }

    if(max < m->shape.y)
    {
        printf("...\n");

        for(int y = m->shape.y - max; y < m->shape.y; y++)
        {
            matrix_print_row(m, y);    
        }
    }    
    printf("] shape=(%d,%d)\n", m->shape.y, m->shape.x);

}

static void matrix_frees(int count, ...)
{
    va_list args;

    // Set the va_list variable with the last fixed argument
    va_start(args, count);

    // Retrieve the arguments and call free
    for (int i = 0; i < count; i++) {
        t_matrix* m = va_arg(args, t_matrix*);
        matrix_free(m);
    }

    // use the va_end to clean va_list variable
    va_end(args);


}

static void matrix_free(t_matrix *m)
{
    if (m != NULL) {
        if (m->data != NULL) {
            free(m->data); 
        }
        free(m);
    }
}

static t_matrix* matrix_dot(t_matrix *m1, t_matrix *m2)
{    
    ASSERT(m1->shape.x == m2->shape.y, "shapes not aligned" );

    t_matrix *res = matrix_create(m1->shape.y, m2->shape.x);
    
    /*
    for(int y1 = 0; y1 < m1->shape.y; y1++)
    {
        for(int x2 = 0; x2 < m2->shape.x; x2++)
        {
            double v = 0;
            for(int x1 = 0; x1 < m1->shape.x; x1++)
            {
                v += m1->data[y1 * m1->shape.x + x1] * m2->data[x1 * m2->shape.x + x2];
            }

            res->data[y1 * res->shape.x + x2] = v;
        }

    }
    */

    /* versione ottimizzata */
    //#pragma omp parallel for // (compilare con -fopenmp)
    for (int i = 0; i < m1->shape.y; i++) {
        int i_m1 = i * m1->shape.x;
        int i_res = i * res->shape.x;

        for (int k = 0; k < m1->shape.x; k++) {
            
            double a = m1->data[i_m1 + k];
            
            // Puntatore alla riga k di m2
            int k_m2 = k * m2->shape.x;

            for (int j = 0; j < m2->shape.x; j++) {
                // ACCESSO LINEARE: j aumenta di 1, la cache ringrazia!
                res->data[i_res + j] += a * m2->data[k_m2 + j];
            }
        }
    }

    return res;
}

/**
 * matrix_matmul_atb
 * Calcola il prodotto C = (A^T) x B
 */
static t_matrix* matrix_matmul_atb(t_matrix *A, t_matrix *B){

    ASSERT(A->shape.y == B->shape.y, "Shapes not aligned for AT * B");

    t_matrix *C = matrix_create(A->shape.x, B->shape.x);
    
    // Algoritmo IKJ adattato per A trasposta
    // i: scorre le colonne di A (che sono le righe di AT)
    // k: scorre le righe di A (che sono le colonne di AT e le righe di B)
    // j: scorre le colonne di B
    for (int i = 0; i < A->shape.x; i++) {
        int row_C = i * C->shape.x;

        for (int k = 0; k < A->shape.y; k++) {
            // Invece di A[i][k], prendiamo A[k][i] che sarebbe AT[i][k]
            double val_AT = A->data[k * A->shape.x + i];
            int row_B = k * B->shape.x;

            for (int j = 0; j < B->shape.x; j++) {
                C->data[row_C + j] += val_AT * B->data[row_B + j];
            }
        }
    }

    return C;
}

static t_matrix* matrix_T(t_matrix *m)
{
    t_matrix* res = matrix_create(m->shape.x, m->shape.y);

    for(int y = 0; y < m->shape.y; y++)
        for(int x = 0; x < m->shape.x; x++)
            res->data[x * m->shape.y + y] = m->data[y * m->shape.x + x];

    return res;
}

static t_matrix* matrix_slice_rows(t_matrix *m, int start, int end)
{
    assert_index(m, start, 0);    
    assert_index(m, end - 1, 0);    

    int n = (end - start) * m->shape.x;
    double *d = malloc(n * sizeof(double));

    memcpy(d, &m->data[start * m->shape.x], n * sizeof(double));

    t_matrix *res = matrix_create_d(d, end - start, m->shape.x);

    /*
    t_matrix res = matrix_create(end - start, m.shape.x);

    for(int y = start; y < end; y++)
        for(int x = 0; x < m.shape.x; x++)
            matrix_setval(res, y - start, x, matrix_getval(m, y, x));
    */
    return res;
}

static t_matrix* matrix_slice_cols(t_matrix *m, int start, int end)
{
    assert_index(m, 0, start);    
    assert_index(m, 0, end - 1);  
    
    t_matrix *res = matrix_create(m->shape.y, end - start);

    for(int y = 0; y < m->shape.y; y++)
        for(int x = start; x < end; x++)
            matrix_setval(res, y, x - start, matrix_getval(m, y, x));


    return res;
}

static t_matrix* matrix_slice(t_matrix *m, int start_y, int end_y, int start_x, int end_x)
{
    assert_index(m, start_y, start_x);    
    assert_index(m, end_y - 1, end_x - 1);  

    t_matrix *res = matrix_create(end_y - start_y, end_x - start_x);

    for(int y = start_y; y < end_y; y++)
        for(int x = start_x; x < end_x; x++)
            matrix_setval(res, y - start_y, x - start_x, matrix_getval(m, y, x));


    return res;

}

static t_matrix* matrix_sum_rows(t_matrix *m)
{
    t_matrix *res = matrix_create(m->shape.y, 1);

    for(int y = 0; y < m->shape.y; y++)
    {
        double sum = 0;

        for(int x = 0; x < m->shape.x; x++)
        {
            sum += matrix_getval(m, y, x);
        }

        matrix_setval(res, y, 0, sum);
    }

    return res;
}

static t_matrix* matrix_sum_cols(t_matrix *m)
{
    t_matrix *res = matrix_create(1, m->shape.x);

    for(int x = 0; x < m->shape.x; x++)
    {
        double sum = 0;

        for(int y = 0; y < m->shape.y; y++)
        {
            sum += matrix_getval(m, y, x);
        }

        matrix_setval(res, 0, x, sum);
    }

    return res;

}

int matrix_equals(t_matrix *m1, t_matrix *m2)
{
    if(m1->size != m2->size)
        return 0;

    if(m1->shape.x != m2->shape.x)
        return 0;

    if(m1->shape.x != m2->shape.x)
        return 0;

    for(int i = 0; i < m1->size; i++)
        if(m1->data[i] != m2->data[i])
            return 0;
    return 1;
}

t_matrix* matrix_range(int range, int sizey, int sizex)
{
    t_matrix *m = matrix_create(sizey, sizex);

    for(int i = 0; i < range && i < m->size; i++)
        m->data[i] = i;

    return m;
}

static t_matrix* matrix_reshape(t_matrix *m, int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    ASSERT(m->size == sizey * sizex, "Dimension mismatch");

    t_matrix *res = matrix_create_d(m->data, sizey, sizex);

    return res;    
}

static double fn_sigmoid(double x)
{
    double s = 1 / (1 + exp(-x));
    return s;
}

static double fn_dsigmoid(double x)
{
    return fn_sigmoid(x)* (1 - fn_sigmoid(x));
}

static double fn_negative(double x)
{
    return -x;
}

static t_matrix* matrix_sumf(t_matrix *m, double f)
{
    t_matrix *res = matrix_create(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] + f;

    return res;
}

static t_matrix* matrix_subf(t_matrix *m, double f)
{
    t_matrix *res = matrix_create(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] - f;

    return res;

}

static t_matrix* matrix_mulf(t_matrix *m, double f)
{
    t_matrix *res = matrix_create(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] * f;

    return res;

}

static t_matrix* matrix_divf(t_matrix *m, double f)
{
    t_matrix *res = matrix_create(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] / f;

    return res;

}

static t_matrix* matrix_fsub(double f, t_matrix *m)
{    
    t_matrix *res = matrix_create(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = f - m->data[i];

    return res;
}

static t_matrix* matrix_sum(t_matrix *m1, t_matrix *m2)
{
    ASSERT(m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y, "matrices must have the same shape");

    t_matrix *res = matrix_create(m1->shape.y, m1->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m1->data[i] + m2->data[i];

    return res;
}

static t_matrix* matrix_sub(t_matrix *m1, t_matrix *m2)
{
    ASSERT(m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y, "matrices must have the same shape");

    t_matrix *res = matrix_create(m1->shape.y, m1->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m1->data[i] - m2->data[i];

    return res;

}

static t_matrix* matrix_mul(t_matrix *m1, t_matrix *m2)
{
    ASSERT(m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y, "matrices must have the same shape");

    t_matrix *res = matrix_create(m1->shape.y, m1->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m1->data[i] * m2->data[i];

    return res;
}

static t_matrix* matrix_div(t_matrix *m1, t_matrix *m2)
{
    ASSERT(m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y, "matrices must have the same shape");

    t_matrix *res = matrix_create(m1->shape.y, m1->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m1->data[i] / m2->data[i];

    return res;

}
