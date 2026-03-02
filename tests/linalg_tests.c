#include "test.h"
#include "../include/linalg.h"
#include "../include/logger.h"
#include <time.h>
#include <math.h>

DEFINE_TEST(multi_dimension_matix_test)
{
    logger_log_info("multi_dimension_matix_test");

    t_LinAlg la = LinAlg_Init();

    // mm is (50, 12288)
    t_matrix *mm = la.range(614400, 50, 12288);

    for(int i = 0; i < mm->shape.y; i++)
    {
        // row is (1, 12228)
        t_matrix *row = la.slice_rows(mm, i, i + 1);

        // r, g, b are (1, 4096)
        t_matrix *r = la.slice_cols(row, 0, 4096);
        t_matrix *g = la.slice_cols(row, 4096, 8192);
        t_matrix *b = la.slice_cols(row, 8192, 12288);

        // rr is (64, 64)
        t_matrix *rr = la.matrixd(r->data, 64, 64);

        la.free(row);

        la.frees(3, r, g, b, rr);

        return NULL;
    }

    la.free(mm);

    return NULL;
}

DEFINE_TEST(matrix_test)
{
    logger_log_info("executing matrix test");
    t_LinAlg la = LinAlg_Init();
    t_matrix *m = la.matrix(3, 2);
    
    test_assert(m->data != NULL, "matrix creation faild");
    test_assert(m->size == 6, "invalid size");
    test_assert(m->shape.x == 2, "invalid shape x");
    test_assert(m->shape.y == 3, "invalid shape y");

    la.free(m);

    return NULL;
}

DEFINE_TEST(matrixd_test)
{
    logger_log_info("executing matrixd test");
    t_LinAlg la = LinAlg_Init();
    double data[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m = la.matrixd(data, 3, 2);
    
    test_assert(m->data != NULL, "matrix creation faild");
    test_assert(m->size == 6, "invalid size");
    test_assert(m->shape.x == 2, "invalid shape x");
    test_assert(m->shape.y == 3, "invalid shape y");

    // Assert dei dati (fondamentale per memcpy)
    for(int i = 0; i < 6; i++) {
        test_assert(m->data[i] == data[i], "Data corruption at index");
    }    
    la.free(m);

    return NULL;
}

DEFINE_TEST(rand_test)
{
    logger_log_info("executing rand test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.rand(10, 10, 3, 5);
    
    test_assert(m->data != NULL, "matrix creation faild");
 
    la.print(m);
    la.free(m);

    return NULL;
}

DEFINE_TEST(equals_test)
{
    logger_log_info("executing equals test");

    t_LinAlg la = LinAlg_Init();
    double data[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m = la.matrixd(data, 3, 2);
    
    test_assert(la.equals(m, m), "matrices are not equal");

    la.free(m);

    return NULL;
}

DEFINE_TEST(dot_test)
{
    logger_log_info("executing dot test");

    t_LinAlg la = LinAlg_Init();
    
    double d1[4] = {1, 2, 3, 4};
    t_matrix *m1 = la.matrixd(d1, 2, 2);

    double d2[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m2 = la.matrixd(d2, 2, 3);

    t_matrix *actual = la.dot(m1, m2);

    double d3[6] = {9, 12, 15, 19, 26, 33};
    t_matrix *expected = la.matrixd(d3, 2, 3);

    test_assert(la.equals(actual, expected), "dot prodoct is not what was expected");

    la.frees(4, m1, m2, actual, expected);

    return NULL;
}

DEFINE_TEST(dot2_test)
{
    logger_log_info("executing dot2 test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.matrix(1000, 1000);

    for(int i = 0; i < m->size; i++)
        m->data[i] = i/1000;

    clock_t start, end;
    double elapsed;

    start = clock();

    t_matrix *m2 = la.dot(m, m);
    end = clock();
    elapsed = (double)(end - start) / (double)CLOCKS_PER_SEC;
    
    logger_log_info("dot matrix (%d,%d)x(%d,%d) ends in %.4f mils", m->shape.x, m->shape.y, m->shape.x, m->shape.y, elapsed * 1000);    

    la.frees(2, m, m2);

    return NULL;
}

DEFINE_TEST(matmul_atb_test)
{
    logger_log_info("executing matmul_atb test");

    t_LinAlg la = LinAlg_Init();
    
    double d1[6] = {1, 5, 4, 9, 3, 8};
    t_matrix *A = la.matrixd(d1, 3, 2);

    double d2[6] = {6, 7, 1, 3, 5, 9};
    t_matrix *B = la.matrixd(d2, 3, 2);

    t_matrix *actual = la.matmul_atb(A, B);

    t_matrix *expected;
    t_matrix *AT;
    AT = la.T(A);
    expected = la.dot(AT, B);

    test_assert(la.equals(actual, expected), "dot prodoct is not what was expected");

    la.frees(5, A, B, AT, actual, expected);

    return NULL;
}

DEFINE_TEST(T_test)
{
    logger_log_info("executing T test");

    t_LinAlg la = LinAlg_Init();

    double d[6] = {1, 2, 3, 4, 5, 6};
    t_matrix *m = la.matrixd(d, 2, 3);

    double d2[6] = {1, 4, 2, 5, 3, 6};
    t_matrix *expected = la.matrixd(d2, 3, 2);

    t_matrix *actual = la.T(m);

    test_assert(la.equals(actual, expected), "Matrix transpose is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(range_test)
{
    logger_log_info("executing range test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *actual = la.range(6, 2, 3);

    double d[6] = {0, 1, 2, 3, 4, 5};
    t_matrix *expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(range2_test)
{
    logger_log_info("executing range2 test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *actual = la.range(3, 2, 3);

    double d[6] = {0, 1, 2, 0, 0, 0};
    t_matrix *expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(range3_test)
{
    logger_log_info("executing range3 test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *actual = la.range(20, 2, 3);

    double d[6] = {0, 1, 2, 3, 4, 5};
    t_matrix *expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_rows_test)
{
    logger_log_info("executing slice_rows test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(12, 4, 3);

    t_matrix *actual = la.slice_rows(m, 1, 2);

    double d[3] = {3, 4, 5};
    t_matrix *expected = la.matrixd(d, 1, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_cols_test)
{
    logger_log_info("executing slice_cols test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(12, 4, 3);

    t_matrix *actual = la.slice_cols(m, 1, 2);

    double d[4] = {1, 4, 7, 10};
    t_matrix *expected = la.matrixd(d, 4, 1);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_test)
{
    logger_log_info("executing slice test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(16, 4, 4);

    t_matrix *actual = la.slice(m, 1, 3, 1, 3);

    double d[4] = {5, 6, 9, 10};
    t_matrix *expected = la.matrixd(d, 2, 2);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    actual = la.slice(m, 1, 4, 1, 2);
    double d2[3] = {5, 9, 13};
    expected = la.matrixd(d2, 3, 1);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

double square(double d)
{
    return d*d;
}

DEFINE_TEST(apply_test)
{
    logger_log_info("executing apply test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *actual = la.range(6, 2, 3);

    la.apply(actual, square);

    double d[6] = {0, 1, 4, 9, 16, 25};
    t_matrix *expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(identity_test)
{
    logger_log_info("executing identity test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *actual = la.identity(3);

    double d[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    t_matrix *expected = la.matrixd(d, 3, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(sum_rows_test)
{
    logger_log_info("executing sum_rows test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(12, 4, 3);

    t_matrix *actual = la.sum_rows(m);
    
    double d[4] = {3, 12, 21, 30};
    t_matrix *expected = la.matrixd(d, 4, 1);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(sum_cols_test)
{
    logger_log_info("executing sum_cols test");

    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(12, 4, 3);

    t_matrix *actual = la.sum_cols(m);
    
    double d[3] = {18, 22, 26};
    t_matrix *expected = la.matrixd(d, 1, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(reshape_test)
{
    logger_log_info("executing reshape test");
    t_LinAlg la = LinAlg_Init();

    double data[6] = {1, 2, 3, 4, 5, 6};

    t_matrix *m = la.matrixd(data, 3, 2);

    t_matrix *actual = la.reshape(m, 2, 3);
    
    t_matrix *expected = la.matrixd(data, 2, 3);
    
    test_assert(actual->data != NULL, "matrix creation faild");
    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(fn_sigmoid_test)
{
    logger_log_info("executing fn_sigmoid test");
    t_LinAlg la = LinAlg_Init();

    double data[2] = {0, 2};
    t_matrix *actual = la.matrixd(data, 1, 2);

    double data_expected[2] = {0.5, (round(0.880797 * 10000)) / 10000};
    t_matrix *expected = la.matrixd(data_expected, 1, 2);

    la.apply(actual, la.fn_sigmoid);
    la.setval(actual, 0, 1, round(la.getval(actual, 0, 1) * 10000) / 10000);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;

}

DEFINE_TEST(sumf_test)
{
    logger_log_info("executing sumf test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(10, 2, 5);
    t_matrix *actual = la.sumf(m, 1);

    double d[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    t_matrix *expected = la.matrixd(d, 2, 5);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(subf_test)
{
    logger_log_info("executing subf test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(10, 2, 5);
    t_matrix *actual = la.subf(m, 1);

    double d[10] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
    t_matrix *expected = la.matrixd(d, 2, 5);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(mulf_test)
{
    logger_log_info("executing mulf test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(3, 1, 3);
    t_matrix *actual = la.mulf(m, 2);

    double d[3] = {0, 2, 4};
    t_matrix *expected = la.matrixd(d, 1, 3);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(divf_test)
{
    logger_log_info("executing divf test");
    t_LinAlg la = LinAlg_Init();

    double da[3] = {2, 4, 6};
    t_matrix *m = la.matrixd(da, 1, 3);
    t_matrix *actual = la.divf(m, 2);
    
    double de[3] = {1, 2, 3};
    t_matrix *expected = la.matrixd(de, 1, 3);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(fsub_test)
{
    logger_log_info("executing fsub test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m = la.range(10, 2, 5);
    t_matrix *actual = la.fsub(1, m);

    double d[10] = { 1, 0, -1, -2, -3, -4, -5, -6, -7, -8};
    t_matrix *expected = la.matrixd(d, 2, 5);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;

}

DEFINE_TEST(sum_test)
{
    logger_log_info("executing sum test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m1 = la.range(3, 1, 3);
    t_matrix *m2 = la.range(3, 1, 3);
    
    t_matrix *actual = la.sum(m1, m2);

    double d[3] = { 0, 2, 4};
    t_matrix *expected = la.matrixd(d, 1, 3);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(4, m1, m2, actual, expected);

    return NULL;

}

DEFINE_TEST(sub_test)
{
    logger_log_info("executing sub test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m1 = la.matrix(1, 3);
    t_matrix *m2 = la.range(3, 1, 3);
    
    t_matrix *actual = la.sub(m1, m2);

    double d[3] = { 0, -1, -2};
    t_matrix *expected = la.matrixd(d, 1, 3);

    test_assert(la.equals(expected, actual), "Matrix is not what was expected");

    la.frees(4, m1, m2, actual, expected);

    return NULL;

}


DEFINE_TEST(all_tests)
{
    test_run(multi_dimension_matix_test);
    test_run(matrix_test);
    test_run(matrixd_test);
    test_run(rand_test);
    test_run(equals_test);
    test_run(dot_test);
    test_run(dot2_test);
    test_run(matmul_atb_test);
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

    return NULL;
}

TEST_SUITE(all_tests);