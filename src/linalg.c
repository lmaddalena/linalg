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

static void assert_index(t_matrix *m, int y, int x);
static void LA_print_row(t_matrix *m, int y, char *fmt);

/* ========================== FUNCTIONS ============================ */

t_matrix* LA_rand(int sizey, int sizex, int lower, int upper)
{
    t_matrix *m = LA_matrix(sizey, sizex);

    srand(time(NULL));

    for(int i = 0; i < m->size; i++)
        m->data[i] =  rand() % (upper - lower + 1) + lower;

    return m;    
}

t_matrix* LA_randf(int sizey, int sizex)
{
    t_matrix *m = LA_matrix(sizey, sizex);

    srand(time(NULL));

    for(int i = 0; i < m->size; i++)
        m->data[i] =  (float)rand() / RAND_MAX;

    return m;    
}

t_matrix* LA_ones(int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    t_matrix* m = LA_matrix(sizey, sizex);

    for(int i = 0; i < m->size; i++)
        m->data[i] = 1;

    return m;    
}

t_matrix* LA_identity(int n)
{
    t_matrix *m = LA_matrix(n, n);

    for(int y = 0; y < n; y ++)
        for(int x = 0; x < n; x++)
            LA_setval(m, y, x, y == x ? 1 : 0);

    return m;
}

void LA_apply(t_matrix *m, double (*fnc)(double d))
{
    for(int i = 0; i < m->size; i++)
        m->data[i] = fnc(m->data[i]);
}

t_matrix* LA_matrix(int sizey, int sizex)
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

t_matrix* LA_matrixd(const double *data, int sizey, int sizex)
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

void assert_index(t_matrix *m, int y, int x)
{
    ASSERT(y >= 0 && y < m->shape.y, "y is out of bound");
    ASSERT(x >= 0 && x < m->shape.x, "x is out of bound");
}

inline void LA_setval(t_matrix *m, int y, int x, double val) 
{
    m->data[y * m->shape.x + x] = val;
}

inline double LA_getval(t_matrix *m, int y, int x) 
{
    return m->data[y * m->shape.x + x];
}

void LA_print_row(t_matrix *m, int y, char *fmt)
{
    int maxcount = 10;
    int max = m->shape.x;

    if(m->shape.x > maxcount)
        max = maxcount / 2;        

    //char format[] = "%.3f";
    
    printf("[");
    for(int x = 0; x < max; x++)
    {
        printf(fmt, LA_getval(m, y, x));
        if(x < m->shape.x -1)
            printf(", ");

    }

    if(max < m->shape.x)
    {
        printf("..., ");
        for(int x = m->shape.x - max; x < m->shape.x; x++)
        {                   
            printf(fmt, LA_getval(m, y, x));
            if(x < m->shape.x -1)
                printf(", ");
        }

    }
    printf("]");
    if(y < m->shape.y -1)
        printf("\n");
}

void LA_print(t_matrix *m)
{
    LA_printf(m, "%.3f");
}

void LA_printf(t_matrix *m, char *fmt)
{
    int maxcount = 10;
    int max = m->shape.y;

    if(m->shape.y > maxcount)
        max = maxcount / 2; 

    printf("[");
    for(int y = 0; y < max; y++)
    {
        LA_print_row(m, y, fmt);
    }

    if(max < m->shape.y)
    {
        printf("...\n");

        for(int y = m->shape.y - max; y < m->shape.y; y++)
        {
            LA_print_row(m, y, fmt);    
        }
    }    
    printf("] shape=(%d,%d)\n", m->shape.y, m->shape.x);

}

void LA_nfree(int count, ...)
{
    va_list args;

    // Set the va_list variable with the last fixed argument
    va_start(args, count);

    // Retrieve the arguments and call free
    for (int i = 0; i < count; i++) {
        t_matrix* m = va_arg(args, t_matrix*);
        LA_free(m);
    }

    // use the va_end to clean va_list variable
    va_end(args);


}

void LA_free(t_matrix *m)
{
    if (m != NULL) {
        if (m->data != NULL) {
            free(m->data); 
        }
        free(m);
    }
}

t_matrix* LA_dot(t_matrix *m1, t_matrix *m2)
{    
    ASSERT(m1->shape.x == m2->shape.y, "shapes not aligned" );

    t_matrix *res = LA_matrix(m1->shape.y, m2->shape.x);
    
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
 * la_dot_atb
 * Calcola il prodotto C = (A^T) x B
 */
t_matrix* LA_dot_atb(t_matrix *A, t_matrix *B){

    ASSERT(A->shape.y == B->shape.y, "Shapes not aligned for AT * B");

    t_matrix *C = LA_matrix(A->shape.x, B->shape.x);
    
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

t_matrix* LA_T(t_matrix *m)
{
    t_matrix* res = LA_matrix(m->shape.x, m->shape.y);

    for(int y = 0; y < m->shape.y; y++)
        for(int x = 0; x < m->shape.x; x++)
            res->data[x * m->shape.y + y] = m->data[y * m->shape.x + x];

    return res;
}

t_matrix* LA_slice_rows(t_matrix *m, int start, int end)
{
    assert_index(m, start, 0);    
    assert_index(m, end - 1, 0);    

    int n = (end - start) * m->shape.x;
    double *d = malloc(n * sizeof(double));

    memcpy(d, &m->data[start * m->shape.x], n * sizeof(double));

    t_matrix *res = LA_matrixd(d, end - start, m->shape.x);

    return res;
}

t_matrix* LA_slice_cols(t_matrix *m, int start, int end)
{
    assert_index(m, 0, start);    
    assert_index(m, 0, end - 1);  
    
    t_matrix *res = LA_matrix(m->shape.y, end - start);

    for(int y = 0; y < m->shape.y; y++)
        for(int x = start; x < end; x++)
            LA_setval(res, y, x - start, LA_getval(m, y, x));


    return res;
}

t_matrix* LA_slice(t_matrix *m, int start_y, int end_y, int start_x, int end_x)
{
    assert_index(m, start_y, start_x);    
    assert_index(m, end_y - 1, end_x - 1);  

    t_matrix *res = LA_matrix(end_y - start_y, end_x - start_x);

    for(int y = start_y; y < end_y; y++)
        for(int x = start_x; x < end_x; x++)
            LA_setval(res, y - start_y, x - start_x, LA_getval(m, y, x));


    return res;

}

t_matrix* LA_sum_rows(t_matrix *m)
{
    t_matrix *res = LA_matrix(m->shape.y, 1);

    for(int y = 0; y < m->shape.y; y++)
    {
        double sum = 0;

        for(int x = 0; x < m->shape.x; x++)
        {
            sum += LA_getval(m, y, x);
        }

        LA_setval(res, y, 0, sum);
    }

    return res;
}

t_matrix* LA_sum_cols(t_matrix *m)
{
    t_matrix *res = LA_matrix(1, m->shape.x);

    for(int x = 0; x < m->shape.x; x++)
    {
        double sum = 0;

        for(int y = 0; y < m->shape.y; y++)
        {
            sum += LA_getval(m, y, x);
        }

        LA_setval(res, 0, x, sum);
    }

    return res;

}

int LA_equals(t_matrix *m1, t_matrix *m2)
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


t_matrix* LA_range(int range, int sizey, int sizex)
{
    t_matrix *m = LA_matrix(sizey, sizex);

    for(int i = 0; i < range && i < m->size; i++)
        m->data[i] = i;

    return m;
}

t_matrix* LA_reshape(t_matrix *m, int sizey, int sizex)
{
    ASSERT(sizex > 0, "x must be upper than 0");
    ASSERT(sizey > 0, "y must be upper than 0");

    ASSERT(m->size == sizey * sizex, "Dimension mismatch");

    t_matrix *res = LA_matrixd(m->data, sizey, sizex);

    return res;    
}

double fn_sigmoid(double x)
{
    double s = 1 / (1 + exp(-x));
    return s;
}

double fn_dsigmoid(double x)
{
    return fn_sigmoid(x)* (1 - fn_sigmoid(x));
}

double fn_negative(double x)
{
    return -x;
}

double fn_relu(double x)
{
    if(x > 0)
        return x;
    else
        return 0;
}

double fn_drelu(double x)
{
    if(x < 0)
        return 0;
    else if(x == 0)
        return 0;   // in x=0 relu derivative is not defined, it is conventionally assigned a value of 0 or 0.5. 
    else
        return 1;
}

t_matrix* LA_sumf(t_matrix *m, double f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] + f;

    return res;
}

t_matrix* LA_subf(t_matrix *m, double f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] - f;

    return res;

}

t_matrix* LA_mulf(t_matrix *m, double f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] * f;

    return res;

}

t_matrix* LA_divf(t_matrix *m, double f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] / f;

    return res;

}

t_matrix* LA_fsub(double f, t_matrix *m)
{    
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = f - m->data[i];

    return res;
}

t_matrix* LA_sum(t_matrix *m1, t_matrix *m2)
{
    int sameshapes = m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y;
    int broadcast_y = m1->shape.x == m2->shape.x && m2->shape.y == 1;
    int broadcast_x = m1->shape.y == m2->shape.y && m2->shape.x == 1;
    
    ASSERT(sameshapes || broadcast_x || broadcast_y, "operands could not be broadcast together");

    t_matrix *res = LA_matrix(m1->shape.y, m1->shape.x);

    for(int y = 0; y < m1->shape.y; y++)
    {
        for(int x = 0; x < m1->shape.x; x++)
        {    
            if(broadcast_y)        
                LA_setval(res, y, x, LA_getval(m1, y, x) + LA_getval(m2, y % m2->shape.y, x));
            else if(broadcast_x)
                LA_setval(res, y, x, LA_getval(m1, y, x) + LA_getval(m2, y, x % m2->shape.x));
            else
                LA_setval(res, y, x, LA_getval(m1, y, x) + LA_getval(m2, y, x));
        }
    }

//    for(int i = 0; i < res->size; i++)
//        res->data[i] = m1->data[i] + m2->data[i];

    return res;
}

t_matrix* LA_sub(t_matrix *m1, t_matrix *m2)
{
    int sameshapes = m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y;
    int broadcast_y = m1->shape.x == m2->shape.x && m2->shape.y == 1;
    int broadcast_x = m1->shape.y == m2->shape.y && m2->shape.x == 1;
    
    ASSERT(sameshapes || broadcast_x || broadcast_y, "operands could not be broadcast together");

    //ASSERT(m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y, "matrices must have the same shape");

    t_matrix *res = LA_matrix(m1->shape.y, m1->shape.x);

    for(int y = 0; y < m1->shape.y; y++)
    {
        for(int x = 0; x < m1->shape.x; x++)
        {    
            if(broadcast_y)        
                LA_setval(res, y, x, LA_getval(m1, y, x) - LA_getval(m2, y % m2->shape.y, x));
            else if(broadcast_x)
                LA_setval(res, y, x, LA_getval(m1, y, x) - LA_getval(m2, y, x % m2->shape.x));
            else
                LA_setval(res, y, x, LA_getval(m1, y, x) - LA_getval(m2, y, x));
        }
    }

    //for(int i = 0; i < res->size; i++)
    //    res->data[i] = m1->data[i] - m2->data[i];

    return res;

}

t_matrix* LA_mul(t_matrix *m1, t_matrix *m2)
{
    int sameshapes = m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y;
    int broadcast_y = m1->shape.x == m2->shape.x && m2->shape.y == 1;
    int broadcast_x = m1->shape.y == m2->shape.y && m2->shape.x == 1;
    
    ASSERT(sameshapes || broadcast_x || broadcast_y, "operands could not be broadcast together");

    //ASSERT(m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y, "matrices must have the same shape");

    t_matrix *res = LA_matrix(m1->shape.y, m1->shape.x);

    for(int y = 0; y < m1->shape.y; y++)
    {
        for(int x = 0; x < m1->shape.x; x++)
        {    
            if(broadcast_y)        
                LA_setval(res, y, x, LA_getval(m1, y, x) * LA_getval(m2, y % m2->shape.y, x));
            else if(broadcast_x)
                LA_setval(res, y, x, LA_getval(m1, y, x) * LA_getval(m2, y, x % m2->shape.x));
            else
                LA_setval(res, y, x, LA_getval(m1, y, x) * LA_getval(m2, y, x));
        }
    }

    //for(int i = 0; i < res->size; i++)
    //    res->data[i] = m1->data[i] * m2->data[i];

    return res;
}

t_matrix* LA_div(t_matrix *m1, t_matrix *m2)
{
    int sameshapes = m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y;
    int broadcast_y = m1->shape.x == m2->shape.x && m2->shape.y == 1;
    int broadcast_x = m1->shape.y == m2->shape.y && m2->shape.x == 1;
    
    ASSERT(sameshapes || broadcast_x || broadcast_y, "operands could not be broadcast together");

    //ASSERT(m1->shape.x == m2->shape.x && m1->shape.y == m2->shape.y, "matrices must have the same shape");

    t_matrix *res = LA_matrix(m1->shape.y, m1->shape.x);

    for(int y = 0; y < m1->shape.y; y++)
    {
        for(int x = 0; x < m1->shape.x; x++)
        {    
            if(broadcast_y)        
                LA_setval(res, y, x, LA_getval(m1, y, x) / LA_getval(m2, y % m2->shape.y, x));
            else if(broadcast_x)
                LA_setval(res, y, x, LA_getval(m1, y, x) / LA_getval(m2, y, x % m2->shape.x));
            else
                LA_setval(res, y, x, LA_getval(m1, y, x) / LA_getval(m2, y, x));
        }
    }


    //for(int i = 0; i < res->size; i++)
    //    res->data[i] = m1->data[i] / m2->data[i];

    return res;
}

t_NN_layer *NN_create_layer(int nunits, int ninputs, enum ACTIVATION_FNC fnc)
{
    t_NN_layer *layer = malloc(sizeof(t_NN_layer));

    layer->W = LA_matrix(ninputs, nunits);
    layer->b = LA_matrix(nunits, 1);
    layer->dW = NULL;
    layer->db = NULL;
    layer->activation = fnc;

    return layer;
}

t_NN_model *NN_create_model(int nlayers, t_NN_layer **layers)
{
    for(int i = nlayers -1; i >= 0; i--)
    {
        t_NN_layer *curr = layers[i];
        t_NN_layer *prev = NULL;

        if(i > 0)
        {
            prev = layers[i - 1];
            ASSERT(curr->W->shape.y == prev->W->shape.x, "invalid layer shape.\nThe shape y of layer n must be equals to shape x of layer n-1.");
        }
    }

    t_NN_model *model = malloc(sizeof(t_NN_model));

    model->layers = layers;
    model->nlayers = nlayers;

    return model;
}

void NN_free_model(t_NN_model *model)
{
    if (model != NULL) {
        if (model->layers != NULL) {
            for(int i = 0; i < model->nlayers; i++)
                NN_free_layer(model->layers[i]); 
        }
        free(model);
    }

}

void NN_free_layer(t_NN_layer *layer)
{
    if (layer != NULL) {
        LA_free(layer->W);
        LA_free(layer->b);
        LA_free(layer->dW);
        LA_free(layer->db);
        free(layer);
    }
}

void NN_print_model(t_NN_model *model)
{
    if(model == NULL)
        return;

    printf("\nMODEL:\n\n");
    
    int totalparams = 0;

    for(int l = 0; l < model->nlayers; l++)
    {
        t_NN_layer *layer = model->layers[l];
        
        int params = layer->W->size + layer->b->size;
        totalparams += params;

        printf("Layer: %d ", l + 1);
        if(l == 0)
            printf("(input)\n");
        else if(l == model->nlayers - 1)
            printf("(output)\n");
        else
            printf("\n");

        printf("-------------------------------\n");

        printf("Shape: (%d, %d)\n", layer->W->shape.y, layer->W->shape.x);
        printf("Params: %d (%.2f KB)\n", params, (double)(params * sizeof(double) / 1024.0));
        printf("Activation function: ");
        switch (layer->activation)
        {
            case SIGMOID :
                printf("sigmoid\n");
                break;
            case RELU :
                printf("ReLU\n");
                break;
            default :
                printf("\n");
        }
        printf("\n");
    }

    printf("Total params: %d (%.2f KB)\n", totalparams, (double)(totalparams * sizeof(double) / 1024.0));

}