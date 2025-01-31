#include "parse_arguments.h"

#include "unity.h"
#include "arguments.h"

void
test_rFlag_is_set(void) {
    arguments_t *args;

    int argc = 2;
    char *argv0 = "tail";
    char *argv1 = "-r";
    char *argv[] = {argv0, argv1};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(true, arguments_get_rFlag(args));

    arguments_free(args);
}

void
test_qFlag_is_set(void) {
    arguments_t *args;

    int argc = 2;
    char *argv0 = "tail";
    char *argv1 = "-q";
    char *argv[] = {argv0, argv1};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(1, arguments_get_qFlag(args));

    arguments_free(args);
}

void
test_when_no_flags_are_set(void) {
    arguments_t *args;

    int argc = 1;
    char *argv0 = "tail";
    char *argv[] = {argv0};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(false, arguments_is_nValue_provided(args));
    TEST_ASSERT_EQUAL(10, arguments_get_n(args));
    TEST_ASSERT_EQUAL(0, arguments_get_qFlag(args));
    TEST_ASSERT_EQUAL(false, arguments_get_rFlag(args));

    arguments_free(args);
}

void
test_numFiles_when_no_file_is_provided(void) {
    arguments_t *args;

    int argc = 1;
    char *arg0 = "tail";
    char *argv[] = {arg0};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(0, arguments_get_numFiles(args));

    arguments_free(args);
}

void
test_numFiles_when_one_file_is_provided(void) {
    arguments_t *args;

    int argc = 2;
    char *arg0 = "tail";
    char *arg1 = "foo";
    char *argv[] = {arg0, arg1};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(1, arguments_get_numFiles(args));

    arguments_free(args);
}

void
test_numFiles_when_multiple_files_are_provided(void) {
    arguments_t *args;

    int argc = 4;
    char *arg0 = "tail";
    char *arg1 = "foo";
    char *arg2 = "bar";
    char *arg3 = "baz";
    char *argv[] = {arg0, arg1, arg2, arg3};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(3, arguments_get_numFiles(args));

    arguments_free(args);
}

void
test_n_value_has_default_directionality(void) {
    arguments_t *args;

    int argc = 3;
    char *arg0 = "tail";
    char *arg1 = "-n";
    char *arg2 = "3";
    char *argv[] = {arg0, arg1, arg2};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(true, arguments_is_nValue_provided(args));
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
    TEST_ASSERT_EQUAL(true, arguments_is_nValue_provided(args));
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
    TEST_ASSERT_EQUAL(true, arguments_is_nValue_provided(args));
    TEST_ASSERT_EQUAL(3, arguments_get_n(args));
    TEST_ASSERT_EQUAL(RELATIVE_TO_BEGINNING, arguments_get_ndirection(args));

    arguments_free(args);
}

void
test_parse_arguments_when_nValue_is_not_provided(void) {
    arguments_t *args;

    int argc = 1;
    char *argv0 = "tail";
    char *argv[] = {argv0};

    args = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(false, arguments_is_nValue_provided(args));

    arguments_free(args);
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_rFlag_is_set);
    RUN_TEST(test_qFlag_is_set);
    RUN_TEST(test_when_no_flags_are_set);
    RUN_TEST(test_numFiles_when_no_file_is_provided);
    RUN_TEST(test_numFiles_when_one_file_is_provided);
    RUN_TEST(test_numFiles_when_multiple_files_are_provided);
    RUN_TEST(test_n_value_has_default_directionality);
    RUN_TEST(test_n_value_has_relative_to_the_end_directionality);
    RUN_TEST(test_n_value_has_relative_to_the_beginning_directionality);
    RUN_TEST(test_parse_arguments_when_nValue_is_not_provided);
    return UNITY_END();
}
