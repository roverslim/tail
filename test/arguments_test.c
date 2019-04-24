#include "unity.h"
#include "arguments.h"

void
test_arguments_get_n_returns_int(void) {
    arguments_t *args;

    args = arguments_init(NULL, NULL, NULL, "13", RELATIVE_TO_END, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(13, arguments_get_n(args));
    arguments_free(args);

    /* Defaults to 10 when n is not specified */
    args = arguments_init(NULL, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(10, arguments_get_n(args));
    arguments_free(args);

    /* Takes the absolute value of any specified value of n */
    args = arguments_init(NULL, NULL, NULL, "-5", RELATIVE_TO_END, 0, 0, 0);
    TEST_ASSERT_EQUAL_INT(5, arguments_get_n(args));
    arguments_free(args);
}

void
test_arguments_get_ndirection_returns_direction_t(void) {
    arguments_t *args;

    args = arguments_init(NULL, NULL, NULL, NULL, RELATIVE_TO_BEGINNING, 0, 0, 0);
    TEST_ASSERT_EQUAL(RELATIVE_TO_BEGINNING, arguments_get_ndirection(args));
    arguments_free(args);

    args = arguments_init(NULL, NULL, NULL, NULL, RELATIVE_TO_END, 0, 0, 0);
    TEST_ASSERT_EQUAL(RELATIVE_TO_END, arguments_get_ndirection(args));
    arguments_free(args);
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_arguments_get_n_returns_int);
    RUN_TEST(test_arguments_get_ndirection_returns_direction_t);
    return UNITY_END();
}
