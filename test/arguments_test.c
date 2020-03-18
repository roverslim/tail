#include "unity.h"
#include "arguments.h"

void
test_arguments_get_rFlag(void) {
    arguments_t *args;

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(0, arguments_get_rFlag(args));
    arguments_free(args);

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 1, 0);
    TEST_ASSERT_EQUAL_INT(1, arguments_get_rFlag(args));
    arguments_free(args);
}

void
test_arguments_get_qFlag(void) {
    arguments_t *args;

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(0, arguments_get_qFlag(args));
    arguments_free(args);

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 1);
    TEST_ASSERT_EQUAL_INT(1, arguments_get_qFlag(args));
    arguments_free(args);
}

void
test_arguments_get_numFiles(void) {
    arguments_t *args;

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(0, arguments_get_numFiles(args));
    arguments_free(args);

    args = arguments_init(NULL, 13, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(13, arguments_get_numFiles(args));
    arguments_free(args);
}

void
test_arguments_get_n(void) {
    arguments_t *args;

    args = arguments_init(NULL, 0, NULL, NULL, "13", RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(13, arguments_get_n(args));
    arguments_free(args);

    /* Defaults to 10 when n is not specified */
    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(10, arguments_get_n(args));
    arguments_free(args);

    /* Takes the absolute value of any specified value of n */
    args = arguments_init(NULL, 0, NULL, NULL, "-5", RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(5, arguments_get_n(args));
    arguments_free(args);
}

void
test_arguments_get_ndirection(void) {
    arguments_t *args;

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_BEGINNING, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL(RELATIVE_TO_BEGINNING, arguments_get_ndirection(args));
    arguments_free(args);

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL(RELATIVE_TO_END, arguments_get_ndirection(args));
    arguments_free(args);
}

void
test_arguments_is_nValue_provided(void) {
    arguments_t *args;

    args = arguments_init(NULL, 0, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL(false, arguments_is_nValue_provided(args));
    arguments_free(args);

    args = arguments_init(NULL, 0, NULL, NULL, "3", RELATIVE_TO_END, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL(true, arguments_is_nValue_provided(args));
    arguments_free(args);
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_arguments_get_rFlag);
    RUN_TEST(test_arguments_get_qFlag);
    RUN_TEST(test_arguments_get_numFiles);
    RUN_TEST(test_arguments_get_n);
    RUN_TEST(test_arguments_get_ndirection);
    RUN_TEST(test_arguments_is_nValue_provided);
    return UNITY_END();
}
