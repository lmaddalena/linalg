#include "../include/linalg.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <omp.h>

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
        m->data[i] =  (float)rand() / (float)RAND_MAX;

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

t_matrix* LA_apply(t_matrix *m, float (*fnc)(float d))
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = fnc(m->data[i]);

    return res;

}

void LA_apply_inplace(t_matrix *m, float (*fnc)(float d))
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
    m->data = calloc(m->size, sizeof(float));

    if (m->data == NULL) {
        free(m);
        ASSERT(0, "out of memory for matrix data");
        return NULL;
    }


    return m;    
}

t_matrix* LA_matrixd(const float *data, int sizey, int sizex)
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
    m->data = malloc(sizeof(float) * m->size);

    if (m->data == NULL) {
        free(m);
        ASSERT(0, "out of memory for matrix data");
        return NULL;
    }

    memcpy(m->data, data, m->size * sizeof(float));

    return m;    
}

void assert_index(t_matrix *m, int y, int x)
{
    ASSERT(y >= 0 && y < m->shape.y, "y is out of bound");
    ASSERT(x >= 0 && x < m->shape.x, "x is out of bound");
}

inline void LA_setval(t_matrix *m, int y, int x, float val) 
{
    m->data[y * m->shape.x + x] = val;
}

inline float LA_getval(t_matrix *m, int y, int x) 
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
            float v = 0;
            for(int x1 = 0; x1 < m1->shape.x; x1++)
            {
                v += m1->data[y1 * m1->shape.x + x1] * m2->data[x1 * m2->shape.x + x2];
            }

            res->data[y1 * res->shape.x + x2] = v;
        }

    }
    */

    /* versione ottimizzata */
    int i;
    #pragma omp parallel for private(i) // (compilare con -fopenmp)
    for (i = 0; i < m1->shape.y; i++) {
        int i_m1 = i * m1->shape.x;
        int i_res = i * res->shape.x;

        for (int k = 0; k < m1->shape.x; k++) {
            
            float a = m1->data[i_m1 + k];
            
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
    int i;
    #pragma omp parallel for private(i) // (compilare con -fopenmp)
    for (i = 0; i < A->shape.x; i++) {
        int row_C = i * C->shape.x;

        for (int k = 0; k < A->shape.y; k++) {
            // Invece di A[i][k], prendiamo A[k][i] che sarebbe AT[i][k]
            float val_AT = A->data[k * A->shape.x + i];
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
    float *d = malloc(n * sizeof(float));

    memcpy(d, &m->data[start * m->shape.x], n * sizeof(float));

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
        float sum = 0;

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
        float sum = 0;

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

    t_matrix *res = LA_matrix(sizey, sizex);

    memcpy(res->data, m->data, m->size * sizeof(float));

    return res;    
}

t_matrix *LA_clone(t_matrix *m)
{

    t_matrix *res = malloc(sizeof(t_matrix));
    if (m == NULL) {
        ASSERT(0, "out of memory for matrix struct");
        return NULL;
    }

    res->shape.x = m->shape.x;
    res->shape.y = m->shape.y;
    res->size = m->shape.x * m->shape.y;
    res->data = malloc(sizeof(float) * m->size);

    if (res->data == NULL) {
        free(res);
        ASSERT(0, "out of memory for matrix data");
        return NULL;
    }

    memcpy(res->data, m->data, m->size * sizeof(float));

    return res;
}

float fn_sigmoid(float x)
{
    float s = 1 / (1 + exp(-x));
    return s;
}

float fn_dsigmoid(float x)
{
    return fn_sigmoid(x)* (1 - fn_sigmoid(x));
}

float fn_negative(float x)
{
    return -x;
}

float fn_relu(float x)
{
    if(x > 0)
        return x;
    else
        return 0;
}

float fn_drelu(float x)
{
    if(x < 0)
        return 0;
    else if(x == 0)
        return 0;   // in x=0 relu derivative is not defined, it is conventionally assigned a value of 0 or 0.5. 
    else
        return 1;
}

float fn_log(float x)
{
    return log(x);
}

t_matrix* LA_sumf(t_matrix *m, float f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] + f;

    return res;
}

t_matrix* LA_subf(t_matrix *m, float f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] - f;

    return res;

}

t_matrix* LA_mulf(t_matrix *m, float f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] * f;

    return res;

}

t_matrix* LA_divf(t_matrix *m, float f)
{
    t_matrix *res = LA_matrix(m->shape.y, m->shape.x);

    for(int i = 0; i < res->size; i++)
        res->data[i] = m->data[i] / f;

    return res;

}

t_matrix* LA_fsub(float f, t_matrix *m)
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

t_matrix *LA_log(t_matrix *m)
{
    t_matrix *res = LA_apply(m, fn_log);
    return res;
}

t_NN_layer *NN_create_layer(int nunits, int ninputs, enum ACTIVATION_FNC fnc)
{
    t_NN_layer *layer = malloc(sizeof(t_NN_layer));

    t_matrix *w_temp = LA_randf(nunits, ninputs);
    t_matrix *b_temp = LA_randf(nunits, 1);

    layer->W = LA_mulf(w_temp, 0.01);
    layer->b = LA_mulf(b_temp, 0.01);
    layer->dW = NULL;
    layer->db = NULL;
    layer->A = NULL;
    layer->Z = NULL;
    layer->activation_fnc = fnc;

    LA_nfree(2, w_temp, b_temp);
    
    return layer;
}

t_NN_model *NN_create_model(int ninputs, int nlayers, struct NN_layer_spec spec[])
{
    t_NN_model *model = malloc(sizeof(t_NN_model));

    model->layers = malloc(sizeof(t_NN_layer) * nlayers);
    model->nlayers = nlayers;

    for(int i = 0; i < nlayers; i++)
    {       
        if(i == 0) // input layer
        {
            model->layers[i] = NN_create_layer(spec[i].nunits, ninputs, spec[i].activation_function);
        }
        else if(i == nlayers - 1) // output layer
        {
            model->layers[i] = NN_create_layer(spec[i].nunits, spec[i-1].nunits, spec[i].activation_function);            
        }
        else // hidden layer
        {
            model->layers[i] = NN_create_layer(spec[i].nunits, spec[i-1].nunits, spec[i].activation_function);            
        }
    }


    return model;

}

t_matrix *NN_model_execute(t_matrix *input, t_NN_model *model)
{
    t_matrix *X = input;

    for(int i = 0; i < model->nlayers; i++)
    {
        float (*fnc)(float d);

        switch (model->layers[i]->activation_fnc)
        {
            case SIGMOID :
                fnc = fn_sigmoid;
                break;
            case RELU :
                fnc = fn_relu;
                break;
            default :
                fnc = fn_relu;
        }

        // Z = WX + b
        t_matrix *Ztemp = LA_dot(model->layers[i]->W, X);
        LA_free(model->layers[i]->Z);
        model->layers[i]->Z = LA_sum(Ztemp, model->layers[i]->b);
        LA_free(model->layers[i]->A);
        model->layers[i]->A = LA_apply(model->layers[i]->Z, fnc);
        X = model->layers[i]->A;

        LA_free(Ztemp);
    }

    t_matrix *Y = model->layers[model->nlayers - 1]->A;

    return Y;
    
}

void NN_free_model(t_NN_model *model)
{
    if (model != NULL) {
        if (model->layers != NULL) {
            for(int i = 0; i < model->nlayers; i++)
                NN_free_layer(model->layers[i]); 
        }
        free(model->layers);
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
        LA_free(layer->A);
        LA_free(layer->Z);

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
        printf("Params: %d (%.2f KB)\n", params, (float)(params * sizeof(float) / 1024.0));
        printf("Activation function: ");
        switch (layer->activation_fnc)
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

    printf("Total params: %d (%.2f KB)\n", totalparams, (float)(totalparams * sizeof(float) / 1024.0));

}

t_matrix *NN_cross_entropy_loss(t_matrix *A, t_matrix *Y)
{
    // A (n, m)
    // Y (n, m)

    int m = A->shape.x;  // number of training example
    int n = A->shape.y;  // number of parameters

    float epsilon = 1e-15; // epsilon smoothing, impedisce che si verifichi log(0)

    t_matrix *J = LA_matrix(n, 1);

    
    for(int k = 0; k < n; k++)
    {
        float j = 0;

        for(int i = 0; i < m; i++)
        {
            float yi = LA_getval(Y, 0, i);
            float ai = LA_getval(A, 0, i);

            j += -yi * log(ai + epsilon) - (1 - yi) * log(1 - ai + epsilon);
        }

        j = j / m;

        LA_setval(J, k, 0, j);
    }

    return J;
}
