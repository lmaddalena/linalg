#include "test.h"
#include "../include/linalg.h"
#include "../include/logger.h"
#include <time.h>

DEFINE_TEST(matrix3d_test)
{
    logger_log_info("executing matrix3d test");
    t_LinAlg la = LinAlg_Init();

    t_matrix *m3d = la.matrix3d(10,10,3);

    for(int i = 0; i < 3; i++)
    {
        test_assert(m3d[i].data != NULL, "matrix creation faild");
        test_assert(m3d[i].size == 100, "invalid size");
        test_assert(m3d[i].shape.x == 10, "invalid shape x");
        test_assert(m3d[i].shape.y == 10, "invalid shape y");
    }

    for(int i = 0; i < 3; i++)
    {
        la.free(m3d[i]);
    }

    return NULL;
}

DEFINE_TEST(matrix_test)
{
    logger_log_info("executing matrix test");
    t_LinAlg la = LinAlg_Init();
    t_matrix m = la.matrix(3, 2);
    
    test_assert(m.data != NULL, "matrix creation faild");
    test_assert(m.size == 6, "invalid size");
    test_assert(m.shape.x == 2, "invalid shape x");
    test_assert(m.shape.y == 3, "invalid shape y");

    la.free(m);

    return NULL;
}

DEFINE_TEST(matrixd_test)
{
    logger_log_info("executing matrixd test");
    t_LinAlg la = LinAlg_Init();
    double data[6] = {1, 2, 3, 4, 5, 6};
    t_matrix m = la.matrixd(data, 3, 2);
    
    test_assert(m.data != NULL, "matrix creation faild");
    test_assert(m.size == 6, "invalid size");
    test_assert(m.shape.x == 2, "invalid shape x");
    test_assert(m.shape.y == 3, "invalid shape y");

    la.free(m);

    return NULL;
}

DEFINE_TEST(equals_test)
{
    logger_log_info("executing equals test");

    t_LinAlg la = LinAlg_Init();
    double data[6] = {1, 2, 3, 4, 5, 6};
    t_matrix m = la.matrixd(data, 3, 2);
    
    test_assert(la.equals(m, m), "matrices are not equal");

    la.free(m);

    return NULL;
}

DEFINE_TEST(dot_test)
{
    logger_log_info("executing dot test");

    t_LinAlg la = LinAlg_Init();
    
    double d1[4] = {1, 2, 3, 4};
    t_matrix m1 = la.matrixd(d1, 2, 2);

    double d2[6] = {1, 2, 3, 4, 5, 6};
    t_matrix m2 = la.matrixd(d2, 2, 3);

    t_matrix actual = la.dot(m1, m2);

    double d3[6] = {9, 12, 15, 19, 26, 33};
    t_matrix expected = la.matrixd(d3, 2, 3);

    test_assert(la.equals(actual, expected), "dot prodoct is not what was expected");

    la.frees(4, m1, m2, actual, expected);

    return NULL;
}

DEFINE_TEST(dot2_test)
{
    logger_log_info("executing dot2 test");

    t_LinAlg la = LinAlg_Init();

    t_matrix m = la.matrix(1000, 1000);

    for(int i = 0; i < m.size; i++)
        m.data[i] = i/1000;

    clock_t start, end;
    double elapsed;

    start = clock();

    t_matrix m2 = la.dot(m, m);
    end = clock();
    elapsed = (double)(end - start) / (double)CLOCKS_PER_SEC;
    
    logger_log_info("dot matrix (%d,%d)x(%d,%d) ends in %.4f mils", m.shape.x, m.shape.y, m.shape.x, m.shape.y, elapsed * 1000);    

    la.frees(2, m, m2);

    return NULL;
}

DEFINE_TEST(T_test)
{
    logger_log_info("executing T test");

    t_LinAlg la = LinAlg_Init();

    double d[6] = {1, 2, 3, 4, 5, 6};
    t_matrix m = la.matrixd(d, 2, 3);

    double d2[6] = {1, 4, 2, 5, 3, 6};
    t_matrix expected = la.matrixd(d2, 3, 2);

    t_matrix actual = la.T(m);

    test_assert(la.equals(actual, expected), "Matrix transpose is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(range_test)
{
    logger_log_info("executing range test");

    t_LinAlg la = LinAlg_Init();

    t_matrix actual = la.range(6, 2, 3);

    double d[6] = {0, 1, 2, 3, 4, 5};
    t_matrix expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(range2_test)
{
    logger_log_info("executing range2 test");

    t_LinAlg la = LinAlg_Init();

    t_matrix actual = la.range(3, 2, 3);

    double d[6] = {0, 1, 2, 0, 0, 0};
    t_matrix expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(range3_test)
{
    logger_log_info("executing range3 test");

    t_LinAlg la = LinAlg_Init();

    t_matrix actual = la.range(20, 2, 3);

    double d[6] = {0, 1, 2, 3, 4, 5};
    t_matrix expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_rows_test)
{
    logger_log_info("executing slice_rows test");

    t_LinAlg la = LinAlg_Init();

    t_matrix m = la.range(12, 4, 3);

    t_matrix actual = la.slice_rows(m, 1, 2);

    double d[3] = {3, 4, 5};
    t_matrix expected = la.matrixd(d, 1, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(slice_cols_test)
{
    logger_log_info("executing slice_cols test");

    t_LinAlg la = LinAlg_Init();

    t_matrix m = la.range(12, 4, 3);

    t_matrix actual = la.slice_cols(m, 1, 2);

    double d[4] = {1, 4, 7, 10};
    t_matrix expected = la.matrixd(d, 4, 1);

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

    t_matrix actual = la.range(6, 2, 3);

    la.apply(actual, square);

    double d[6] = {0, 1, 4, 9, 16, 25};
    t_matrix expected = la.matrixd(d, 2, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}

DEFINE_TEST(identity_test)
{
    logger_log_info("executing identity test");

    t_LinAlg la = LinAlg_Init();

    t_matrix actual = la.identity(3);

    double d[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    t_matrix expected = la.matrixd(d, 3, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(2, actual, expected);

    return NULL;
}


DEFINE_TEST(sum_rows_test)
{
    logger_log_info("executing sum_rows test");

    t_LinAlg la = LinAlg_Init();

    t_matrix m = la.range(12, 4, 3);

    t_matrix actual = la.sum_rows(m);
    
    double d[4] = {3, 12, 21, 30};
    t_matrix expected = la.matrixd(d, 4, 1);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(sum_cols_test)
{
    logger_log_info("executing sum_cols test");

    t_LinAlg la = LinAlg_Init();

    t_matrix m = la.range(12, 4, 3);

    t_matrix actual = la.sum_cols(m);
    
    double d[3] = {18, 22, 26};
    t_matrix expected = la.matrixd(d, 1, 3);

    test_assert(la.equals(actual, expected), "Matrix is not what was expected");

    la.frees(3, m, actual, expected);

    return NULL;
}

DEFINE_TEST(all_tests)
{
    test_run(matrix3d_test);
    test_run(matrix_test);
    test_run(matrixd_test);
    test_run(equals_test);
    test_run(dot_test);
    test_run(dot2_test);
    test_run(T_test);
    test_run(range_test);
    test_run(range2_test);
    test_run(range3_test);
    test_run(slice_rows_test);
    test_run(slice_cols_test);
    test_run(apply_test);
    test_run(identity_test);
    test_run(sum_rows_test);
    test_run(sum_cols_test);

    return NULL;
}

TEST_SUITE(all_tests);