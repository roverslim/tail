#include "unity.h"
#include "parse_arguments.h"

void
test_n_value_has_default_directionality(void) {
    arguments_t *args;

    int argc = 3;
    char *arg0 = "tail";
    char *arg1 = "-n";
    char *arg2 = "3";
    char *argv[] = {arg0, arg1, arg2};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(3, arguments_get_n(args));
    TEST_ASSERT_EQUAL(RELATIVE_TO_END, arguments_get_ndirection(args));

    arguments_free(args);
}

void
test_n_value_has_relative_to_the_end_directionality(void) {
    arguments_t *args;

    int argc = 3;
    char *arg0 = "tail";
    char *arg1 = "-n";
    char *arg2 = "-3";
    char *argv[] = {arg0, arg1, arg2};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(3, arguments_get_n(args));
    TEST_ASSERT_EQUAL(RELATIVE_TO_END, arguments_get_ndirection(args));

    arguments_free(args);
}

void
test_n_value_has_relative_to_the_beginning_directionality(void) {
    arguments_t *args;

    int argc = 3;
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+3";
    char *argv[] = {argv0, argv1, argv2};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(3, arguments_get_n(args));
    TEST_ASSERT_EQUAL(RELATIVE_TO_BEGINNING, arguments_get_ndirection(args));

    arguments_free(args);
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_n_value_has_default_directionality);
    RUN_TEST(test_n_value_has_relative_to_the_end_directionality);
    RUN_TEST(test_n_value_has_relative_to_the_beginning_directionality);
    return UNITY_END();
}
