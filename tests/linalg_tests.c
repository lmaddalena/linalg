#include "test.h"
#include "../include/linalg.h"
#include "../include/logger.h"
#include <time.h>
#include <math.h>

DEFINE_TEST(multi_dimension_matix_test)
{
    logger_log_info("multi_dimension_matix_test");

    // mm is (50, 12288)
    t_matrix *mm = LA_range(614400, 50, 12288);

    for(int i = 0; i < mm->shape.y; i++)
    {
        // row is (1, 12228)
        t_matrix *row = LA_slice_rows(mm, i, i + 1);

        // r, g, b are (1, 4096)
        t_matrix *r = LA_slice_cols(row, 0, 4096);
        t_matrix *g = LA_slice_cols(row, 4096, 8192);
        t_matrix *b = LA_slice_cols(row, 8192, 12288);

        // rr is (64, 64)
        t_matrix *rr = LA_matrixd(r->data, 64, 64);

        LA_free(row);

        LA_frees(3, r, g, b, rr);

        return NULL;
    }

    LA_free(mm);

    return NULL;
}

DEFINE_TEST(matrix_test)
{
    logger_log_info("executing matrix test");
    t_matrix *m = LA_matrix(3, 2);
    
    test_assert(m->data != NULL, "matrix creation faild");
    test_assert(m->size == 6, "invalid size");
    test_assert(m->shape.x == 2, "invalid shape x");
    test_assert(m->shape.y == 3, "invalid shape y");

    LA_free(m);

    return NULL;
}

DEFINE_TEST(matrixd_test)
{
    logger_log_info("executing matrixd test");

    double data[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m = LA_matrixd(data, 3, 2);
    
    test_assert(m->data != NULL, "matrix creation faild");
    test_assert(m->size == 6, "invalid size");
    test_assert(m->shape.x == 2, "invalid shape x");
    test_assert(m->shape.y == 3, "invalid shape y");

    // Assert dei dati (fondamentale per memcpy)
    for(int i = 0; i < 6; i++) {
        test_assert(m->data[i] == data[i], "Data corruption at index");
    }    
    LA_free(m);

    return NULL;
}

DEFINE_TEST(rand_test)
{
    logger_log_info("executing rand test");

    t_matrix *m = LA_rand(10, 10, 3, 5);
    
    test_assert(m->data != NULL, "matrix creation faild");
 
    LA_free(m);

    return NULL;
}

DEFINE_TEST(randf_test)
{
    logger_log_info("executing randf test");

    t_matrix *m = LA_randf(10, 10);

    test_assert(m->data != NULL, "matrix creation faild");
 
    LA_free(m);

    return NULL;
}

DEFINE_TEST(equals_test)
{
    logger_log_info("executing equals test");

    double data[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m = LA_matrixd(data, 3, 2);
    
    test_assert(LA_equals(m, m), "matrices are not equal");

    LA_free(m);

    return NULL;
}

DEFINE_TEST(dot_test)
{
    logger_log_info("executing dot test");
   
    double d1[4] = {1, 2, 3, 4};
    t_matrix *m1 = LA_matrixd(d1, 2, 2);

    double d2[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m2 = LA_matrixd(d2, 2, 3);

    t_matrix *actual = LA_dot(m1, m2);

    double d3[6] = {9, 12, 15, 19, 26, 33};
    t_matrix *expected = LA_matrixd(d3, 2, 3);

    test_assert(LA_equals(actual, expected), "dot prodoct is not what was expected");

    LA_frees(4, m1, m2, actual, expected);

    return NULL;
}

DEFINE_TEST(dot2_test)
{
    logger_log_info("executing dot2 test");

    t_matrix *m = LA_matrix(1000, 1000);

    for(int i = 0; i < m->size; i++)
        m->data[i] = i/1000;

    clock_t start, end;
    double elapsed;

    start = clock();

    t_matrix *m2 = LA_dot(m, m);
    end = clock();
    elapsed = (double)(end - start) / (double)CLOCKS_PER_SEC;
    
    logger_log_info("dot matrix (%d,%d)x(%d,%d) ends in %.4f mils", m->shape.x, m->shape.y, m->shape.x, m->shape.y, elapsed * 1000);    

    LA_frees(2, m, m2);

    return NULL;
}

DEFINE_TEST(dot_atb_test)
{
    logger_log_info("executing dot_atb test");
   
    double d1[6] = {1, 5, 4, 9, 3, 8};
    t_matrix *A = LA_matrixd(d1, 3, 2);

    double d2[6] = {6, 7, 1, 3, 5, 9};
    t_matrix *B = LA_matrixd(d2, 3, 2);

    t_matrix *actual = LA_dot_atb(A, B);

    t_matrix *expected;
    t_matrix *AT;
    AT = LA_T(A);
    expected = LA_dot(AT, B);

    test_assert(LA_equals(actual, expected), "dot prodoct is not what was expected");

    LA_frees(5, A, B, AT, actual, expected);

    return NULL;
}

DEFINE_TEST(T_test)
{
    logger_log_info("executing T test");

    double d[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m = LA_matrixd(d, 2, 3);

    double d2[6] = {1, 4, 2, 5, 3, 6};
    t_matrix *expected = LA_matrixd(d2, 3, 2);

    t_matrix *actual = LA_T(m);

    test_assert(LA_equals(actual, expected), "Matrix transpose is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(range_test)
{
    logger_log_info("executing range test");

    t_matrix *actual = LA_range(6, 2, 3);

    double d[6] = {0, 1, 2, 3, 4, 5};
    t_matrix *expected = LA_matrixd(d, 2, 3);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(range2_test)
{
    logger_log_info("executing range2 test");

    t_matrix *actual = LA_range(3, 2, 3);

    double d[6] = {0, 1, 2, 0, 0, 0};
    t_matrix *expected = LA_matrixd(d, 2, 3);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(range3_test)
{
    logger_log_info("executing range3 test");

    t_matrix *actual = LA_range(20, 2, 3);

    double d[6] = {0, 1, 2, 3, 4, 5};
    t_matrix *expected = LA_matrixd(d, 2, 3);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_rows_test)
{
    logger_log_info("executing slice_rows test");

    t_matrix *m = LA_range(12, 4, 3);

    t_matrix *actual = LA_slice_rows(m, 1, 2);

    double d[3] = {3, 4, 5};
    t_matrix *expected = LA_matrixd(d, 1, 3);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_cols_test)
{
    logger_log_info("executing slice_cols test");

    t_matrix *m = LA_range(12, 4, 3);

    t_matrix *actual = LA_slice_cols(m, 1, 2);

    double d[4] = {1, 4, 7, 10};
    t_matrix *expected = LA_matrixd(d, 4, 1);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_test)
{
    logger_log_info("executing slice test");

    t_matrix *m = LA_range(16, 4, 4);

    t_matrix *actual = LA_slice(m, 1, 3, 1, 3);

    double d[4] = {5, 6, 9, 10};
    t_matrix *expected = LA_matrixd(d, 2, 2);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(2, actual, expected);

    actual = LA_slice(m, 1, 4, 1, 2);
    double d2[3] = {5, 9, 13};
    expected = LA_matrixd(d2, 3, 1);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;
}

double square(double d)
{
    return d*d;
}

DEFINE_TEST(apply_test)
{
    logger_log_info("executing apply test");

    t_matrix *actual = LA_range(6, 2, 3);

    LA_apply(actual, square);

    double d[6] = {0, 1, 4, 9, 16, 25};
    t_matrix *expected = LA_matrixd(d, 2, 3);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(identity_test)
{
    logger_log_info("executing identity test");

    t_matrix *actual = LA_identity(3);

    double d[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    t_matrix *expected = LA_matrixd(d, 3, 3);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(sum_rows_test)
{
    logger_log_info("executing sum_rows test");

    t_matrix *m = LA_range(12, 4, 3);

    t_matrix *actual = LA_sum_rows(m);
    
    double d[4] = {3, 12, 21, 30};
    t_matrix *expected = LA_matrixd(d, 4, 1);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(sum_cols_test)
{
    logger_log_info("executing sum_cols test");

    t_matrix *m = LA_range(12, 4, 3);

    t_matrix *actual = LA_sum_cols(m);
    
    double d[3] = {18, 22, 26};
    t_matrix *expected = LA_matrixd(d, 1, 3);

    test_assert(LA_equals(actual, expected), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(reshape_test)
{
    logger_log_info("executing reshape test");

    double data[6] = {1, 2, 3, 4, 5, 6};

    t_matrix *m = LA_matrixd(data, 3, 2);

    t_matrix *actual = LA_reshape(m, 2, 3);
    
    t_matrix *expected = LA_matrixd(data, 2, 3);
    
    test_assert(actual->data != NULL, "matrix creation faild");
    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(fn_sigmoid_test)
{
    logger_log_info("executing fn_sigmoid test");

    double data[2] = {0, 2};
    t_matrix *actual = LA_matrixd(data, 1, 2);

    double data_expected[2] = {0.5, (round(0.880797 * 10000)) / 10000};
    t_matrix *expected = LA_matrixd(data_expected, 1, 2);

    LA_apply(actual, fn_sigmoid);
    LA_setval(actual, 0, 1, round(LA_getval(actual, 0, 1) * 10000) / 10000);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(2, actual, expected);

    return NULL;

}

DEFINE_TEST(sumf_test)
{
    logger_log_info("executing sumf test");
    t_matrix *m = LA_range(10, 2, 5);
    t_matrix *actual = LA_sumf(m, 1);

    double d[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    t_matrix *expected = LA_matrixd(d, 2, 5);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(subf_test)
{
    logger_log_info("executing subf test");

    t_matrix *m = LA_range(10, 2, 5);
    t_matrix *actual = LA_subf(m, 1);

    double d[10] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
    t_matrix *expected = LA_matrixd(d, 2, 5);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(mulf_test)
{
    logger_log_info("executing mulf test");

    t_matrix *m = LA_range(3, 1, 3);
    t_matrix *actual = LA_mulf(m, 2);

    double d[3] = {0, 2, 4};
    t_matrix *expected = LA_matrixd(d, 1, 3);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(divf_test)
{
    logger_log_info("executing divf test");

    double da[3] = {2, 4, 6};
    t_matrix *m = LA_matrixd(da, 1, 3);
    t_matrix *actual = LA_divf(m, 2);
    
    double de[3] = {1, 2, 3};
    t_matrix *expected = LA_matrixd(de, 1, 3);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(fsub_test)
{
    logger_log_info("executing fsub test");

    t_matrix *m = LA_range(10, 2, 5);
    t_matrix *actual = LA_fsub(1, m);

    double d[10] = { 1, 0, -1, -2, -3, -4, -5, -6, -7, -8};
    t_matrix *expected = LA_matrixd(d, 2, 5);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(sum_test)
{
    logger_log_info("executing sum test");

    t_matrix *m1 = LA_range(3, 1, 3);
    t_matrix *m2 = LA_range(3, 1, 3);
    
    t_matrix *actual = LA_sum(m1, m2);

    double d[3] = { 0, 2, 4};
    t_matrix *expected = LA_matrixd(d, 1, 3);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(4, m1, m2, actual, expected);

    return NULL;

}

DEFINE_TEST(sub_test)
{
    logger_log_info("executing sub test");

    t_matrix *m1 = LA_matrix(1, 3);
    t_matrix *m2 = LA_range(3, 1, 3);
    
    t_matrix *actual = LA_sub(m1, m2);

    double d[3] = { 0, -1, -2};
    t_matrix *expected = LA_matrixd(d, 1, 3);

    test_assert(LA_equals(expected, actual), "Matrix is not what was expected");

    LA_frees(4, m1, m2, actual, expected);

    return NULL;

}

DEFINE_TEST(NN_create_model_test)
{
    logger_log_info("executing NN_create_model_test test");

    t_NN_layer *input = NN_create_layer(32, 4, RELU);
    t_NN_layer *hidden = NN_create_layer(32, 32, RELU);
    t_NN_layer *output = NN_create_layer(3, 32, SIGMOID);

    t_NN_layer *layers[3] = {input, hidden, output};

    t_NN_model *model = NN_create_model(3, layers);

    NN_print_model(model);

    NN_free_model(model);

    return NULL;

}
DEFINE_TEST(all_tests)
{
    test_run(multi_dimension_matix_test);
    test_run(matrix_test);
    test_run(matrixd_test);
    test_run(rand_test);
    test_run(randf_test);
    test_run(equals_test);
    test_run(dot_test);
    test_run(dot2_test);
    test_run(dot_atb_test);
    test_run(T_test);
    test_run(range_test);
    test_run(range2_test);
    test_run(range3_test);
    test_run(slice_test);
    test_run(slice_rows_test);
    test_run(slice_cols_test);
    test_run(apply_test);
    test_run(identity_test);
    test_run(sum_rows_test);
    test_run(sum_cols_test);
    test_run(reshape_test);
    test_run(fn_sigmoid_test);
    test_run(sumf_test);
    test_run(subf_test);
    test_run(mulf_test);
    test_run(divf_test);
    test_run(fsub_test);
    test_run(sum_test);
    test_run(sub_test);
    test_run(NN_create_model_test);

    return NULL;
}

TEST_SUITE(all_tests);