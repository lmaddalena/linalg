#include "../include/linalg.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

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

static t_matrix matrix_create(int sizey, int sizex);
static t_matrix matrix_create_d(double *data, int sizey, int sizex);
static void assert_index(t_matrix m, int y, int x);
static void matrix_setval(t_matrix m, int y, int x, double val);
static double matrix_getval(t_matrix m, int y, int x);
static void matrix_print_row(t_matrix m, int y);
static void matrix_print(t_matrix m);
static void matrix_frees(int count, ...);
static void matrix_free(t_matrix m);
static t_matrix matrix_dot(t_matrix m1, t_matrix m2);
static t_matrix matrix_T(t_matrix m);
static t_matrix matrix_slice_rows(t_matrix m, int start, int end);
static t_matrix matrix_slice_cols(t_matrix m, int start, int end);
static void matrix_apply(t_matrix m, double (*fnc)(double d));
static t_matrix matrix_identity(int n);
static t_matrix matrix_ones(int sizey, int sizex);
static t_matrix matrix_rand(int sizey, int sizex, int lower, int upper);
static t_matrix matrix_sum_rows(t_matrix m);
static t_matrix matrix_sum_cols(t_matrix m);
static int matrix_equals(t_matrix m1, t_matrix m2);
static t_matrix matrix_range(int range, int sizey, int sizex);
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
    la.frees = matrix_frees;
    la.free = matrix_free;
    la.getval = matrix_getval;
    la.setval = matrix_setval;
    la.print = matrix_print;
    la.T = matrix_T;
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

    return la;
}


/* ========================== STATIC FUNCTIONS ============================ */

static t_matrix matrix_rand(int sizey, int sizex, int lower, int upper)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    t_matrix m;

    m.shape.x = sizex;
    m.shape.y = sizey;
    m.size = sizex * sizey;
    m.data = (double *)malloc(m.size * sizeof(double));

    ASSERT(m.data != NULL, "out of memory");

    srand(time(NULL));

    for(int i = 0; i < m.size; i++)
        m.data[i] =  (rand() % upper) + lower;

    return m;    
}

static t_matrix matrix_ones(int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    t_matrix m;

    m.shape.x = sizex;
    m.shape.y = sizey;
    m.size = sizex * sizey;
    m.data = (double *)malloc(m.size * sizeof(double));

    ASSERT(m.data != NULL, "out of memory");

    for(int i = 0; i < m.size; i++)
        m.data[i] = 1;

    return m;    
}

static t_matrix matrix_identity(int n)
{
    t_matrix m;

    m.shape.x = n;
    m.shape.y = n;
    m.size = n * n;
    m.data = (double *)malloc(m.size * sizeof(double));


    for(int y = 0; y < n; y ++)
        for(int x = 0; x < n; x++)
            matrix_setval(m, y, x, y == x ? 1 : 0);

    return m;
}

static void matrix_apply(t_matrix m, double (*fnc)(double d))
{
    for(int i = 0; i < m.size; i++)
        m.data[i] = fnc(m.data[i]);
}

static t_matrix matrix_create(int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    t_matrix m;

    m.shape.x = sizex;
    m.shape.y = sizey;
    m.size = sizex * sizey;
    m.data = (double *)calloc(m.size, sizeof(double));

    ASSERT(m.data != NULL, "out of memory");

    return m;    
}

static t_matrix matrix_create_d(double *data, int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    t_matrix m;

    m.shape.x = sizex;
    m.shape.y = sizey;
    m.size = sizex * sizey;
    m.data = (double *)calloc(m.size, sizeof(double));

    ASSERT(m.data != NULL, "out of memory");
    
    for(int i = 0; i < m.size; i++)
        m.data[i] = data[i];

    return m;    
}

static void assert_index(t_matrix m, int y, int x)
{
    ASSERT(y >= 0 && y < m.shape.y, "y is out of bound");
    ASSERT(x >= 0 && x < m.shape.x, "x is out of bound");
}

static void matrix_setval(t_matrix m, int y, int x, double val) 
{
    assert_index(m, y, x);
    m.data[y * m.shape.x + x] = val;
}

static double matrix_getval(t_matrix m, int y, int x) 
{
    assert_index(m, y, x);
    return m.data[y * m.shape.x + x];
}

static void matrix_print_row(t_matrix m, int y)
{
    int maxcount = 10;
    int max = m.shape.x;

    if(m.shape.x > maxcount)
        max = maxcount / 2;        

    char format[] = "%.3f";
    
    printf("[");
    for(int x = 0; x < max; x++)
    {
        printf(format, matrix_getval(m, y, x));
        if(x < m.shape.x -1)
            printf(", ");

    }

    if(max < m.shape.x)
    {
        printf("..., ");
        for(int x = m.shape.x - max; x < m.shape.x; x++)
        {                   
            printf("%.3f", matrix_getval(m, y, x));
            if(x < m.shape.x -1)
                printf(", ");
        }

    }
    printf("]");
    if(y < m.shape.y -1)
        printf("\n");
}

static void matrix_print(t_matrix m)
{
    int maxcount = 10;
    int max = m.shape.y;

    if(m.shape.y > maxcount)
        max = maxcount / 2; 

    printf("[");
    for(int y = 0; y < max; y++)
    {
        matrix_print_row(m, y);
    }

    if(max < m.shape.y)
    {
        printf("...\n");

        for(int y = m.shape.y - max; y < m.shape.y; y++)
        {
            matrix_print_row(m, y);    
        }
    }    
    printf("] shape=(%d,%d)\n", m.shape.y, m.shape.x);

}

static void matrix_frees(int count, ...)
{
    va_list args;

    // Set the va_list variable with the last fixed argument
    va_start(args, count);

    // Retrieve the arguments and call free
    for (int i = 0; i < count; i++) {
        t_matrix m = va_arg(args, t_matrix);
        free(m.data);
    }

    // use the va_end to clean va_list variable
    va_end(args);


}

static void matrix_free(t_matrix m)
{
        free(m.data);
}

static t_matrix matrix_dot(t_matrix m1, t_matrix m2)
{    
    ASSERT(m1.shape.x == m2.shape.y, "shapes not aligned" );

    t_matrix res = matrix_create(m1.shape.y, m2.shape.x);

    for(int y1 = 0; y1 < m1.shape.y; y1++)
    {
        for(int x2 = 0; x2 < m2.shape.x; x2++)
        {
            double v = 0;
            for(int x1 = 0; x1 < m1.shape.x; x1++)
            {
                //v = v + matrix_getval(m1, y1, x1) * matrix_getval(m2, x1, x2);
                v += m1.data[y1 * m1.shape.x + x1] * m2.data[x1 * m2.shape.x + x2];
            }

            //matrix_setval(res, y1, x2, v);
            res.data[y1 * res.shape.x + x2] = v;
        }

    }
    return res;
}

static t_matrix matrix_T(t_matrix m)
{
    t_matrix res = matrix_create(m.shape.x, m.shape.y);

    for(int y = 0; y < m.shape.y; y++)
        for(int x = 0; x < m.shape.x; x++)
            matrix_setval(res, x, y, matrix_getval(m, y, x));

    return res;
}

static t_matrix matrix_slice_rows(t_matrix m, int start, int end)
{
    assert_index(m, start, 0);    
    assert_index(m, end - 1, 0);    

    t_matrix res = matrix_create(end - start, m.shape.x);

    for(int y = start; y < end; y++)
        for(int x = 0; x < m.shape.x; x++)
            matrix_setval(res, y - start, x, matrix_getval(m, y, x));

    return res;
}

static t_matrix matrix_slice_cols(t_matrix m, int start, int end)
{
    assert_index(m, 0, start);    
    assert_index(m, 0, end - 1);  
    
    t_matrix res = matrix_create(m.shape.y, end - start);

    for(int y = 0; y < m.shape.y; y++)
        for(int x = start; x < end; x++)
            matrix_setval(res, y, x - start, matrix_getval(m, y, x));


    return res;
}

static t_matrix matrix_sum_rows(t_matrix m)
{
    t_matrix res = matrix_create(m.shape.y, 1);

    for(int y = 0; y < m.shape.y; y++)
    {
        int sum = 0;

        for(int x = 0; x < m.shape.x; x++)
        {
            sum += matrix_getval(m, y, x);
        }

        matrix_setval(res, y, 0, sum);
    }

    return res;
}

static t_matrix matrix_sum_cols(t_matrix m)
{
    t_matrix res = matrix_create(1, m.shape.x);

    for(int x = 0; x < m.shape.x; x++)
    {
        int sum = 0;

        for(int y = 0; y < m.shape.y; y++)
        {
            sum += matrix_getval(m, y, x);
        }

        matrix_setval(res, 0, x, sum);
    }

    return res;

}

int matrix_equals(t_matrix m1, t_matrix m2)
{
    if(m1.size != m2.size)
        return 0;

    if(m1.shape.x != m2.shape.x)
        return 0;

    if(m1.shape.x != m2.shape.x)
        return 0;

    for(int i = 0; i < m1.size; i++)
        if(m1.data[i] != m2.data[i])
            return 0;
    return 1;
}

t_matrix matrix_range(int range, int sizey, int sizex)
{
    t_matrix m = matrix_create(sizey, sizex);

    for(int i = 0; i < range && i < m.size; i++)
        m.data[i] = i;

    return m;
}
