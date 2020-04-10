#include "tail.h"

#include <time.h>
#include "unity.h"

int
convert_to_argc(const char *command) {
    int argc;

    TEST_ASSERT_NOT_NULL(command);
    argc = 1;
    for (int i=0; i < strlen(command); i++)
        if (' ' == command[i])
            argc++;

    return argc;
}

char **
convert_to_argv(int argc, const char *command) {
    char **argv, *command_copy, *token;

    argv = (char**) malloc((argc + 1) * sizeof(char *));
    TEST_ASSERT_NOT_NULL(argv);
    for (int i=0; i <= argc; i++)
        argv[i] = NULL;

    TEST_ASSERT_NOT_NULL(command);
    command_copy = (char*) calloc(strlen(command) + 1, sizeof(char));
    TEST_ASSERT_NOT_NULL(command_copy);
    strcpy(command_copy, command);

    token = strtok(command_copy, " ");
    for (int i=0; token != NULL; i++) {
        argv[i] = calloc(strlen(token) + 1, sizeof(char));
        TEST_ASSERT_NOT_NULL(argv[i]);
        strcpy(argv[i], token);

        token = strtok(NULL, " ");
    }
    free(command_copy);

    return argv;
}

void
free_argv(char **argv) {
    if (NULL == argv) return;

    for (int i=0; NULL != argv[i]; i++)
        free(argv[i]);
    free(argv);
}

void
test_assert_tail(
      int expectedStatus, const char *expected_output,
      const char *command
) {
    char **argv, *output, testFilename[255];
    FILE *fp;
    int argc, status;
    long length;

    TEST_ASSERT_NOT_NULL(command);

    srand(time(0));
    sprintf(testFilename, "/tmp/tail_test.%d", rand());
    fp = fopen(testFilename, "w+");
    TEST_ASSERT_NOT_NULL(fp);

    argc = convert_to_argc(command);
    argv = convert_to_argv(argc, command);
    TEST_ASSERT_NOT_NULL(argv);

    status = tail(argc, argv, fp);
    free_argv(argv);

    fseek(fp, 0L, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    output = (char *) calloc(length + 1, sizeof(char));
    TEST_ASSERT_NOT_NULL(output);
    fread(output, sizeof(char), length, fp);
    fclose(fp);
    remove(testFilename);

    TEST_ASSERT_EQUAL(expectedStatus, status);
    TEST_ASSERT_EQUAL_STRING(expected_output, output);
    free(output);
}

void
test_tail_when_file_does_not_exist(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail no-such-file";
    expected_output = "tail: no-such-file: No such file or directory\n";
    expected_status = -1;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_with_default_arguments(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail test/data/short-sample.txt";
    expected_output = "\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n\
line 1\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_q_is_set_with_a_single_input_file(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -q test/data/short-sample.txt";
    expected_output = "\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_with_default_arguments_and_multiple_input_files(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail test/data/short-sample.txt test/data/empty-file.txt";
    expected_output = "\
==> test/data/short-sample.txt <==\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n\
\n\
==> test/data/empty-file.txt <==\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_q_is_set_with_multiple_input_files(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -q test/data/short-sample.txt test/data/empty-file.txt";
    expected_output = "\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_zero(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 0 test/data/short-sample.txt";
    expected_output = "";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_zero_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 0 -r test/data/short-sample.txt";
    expected_output = "";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_zero(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -0 test/data/short-sample.txt";
    expected_output = "";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_zero_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status = 0;

    command = "tail -n 0 -r test/data/short-sample.txt";
    expected_output = "";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_zero(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +0 test/data/short-sample.txt";
    expected_output = "\
line 1\n\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_zero_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +0 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n\
line 1\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_one(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 1 test/data/short-sample.txt";
    expected_output = "line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_one_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 1 -r test/data/short-sample.txt";
    expected_output = "line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_one(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -1 test/data/short-sample.txt";
    expected_output = "line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_one_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -1 -r test/data/short-sample.txt";
    expected_output = "line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_one(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +1 test/data/short-sample.txt";
    expected_output = "\
line 1\n\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_one_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +1 -r test/data/short-sample.txt";
    expected_output = "";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_two(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 2 test/data/short-sample.txt";
    expected_output = "\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_two_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 2 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_two(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -2 test/data/short-sample.txt";
    expected_output = "\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_two_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -2 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_two(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +2 test/data/short-sample.txt";
    expected_output = "\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_two_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +2 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_eleven(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 11 test/data/short-sample.txt";
    expected_output = "\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_eleven_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 11 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_eleven(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -11 test/data/short-sample.txt";
    expected_output = "\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_eleven_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -11 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_eleven(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +11 test/data/short-sample.txt";
    expected_output = "\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_eleven_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +11 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_twelve(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 12 test/data/short-sample.txt";
    expected_output = "\
line 1\n\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_twelve_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 12 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n\
line 1\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_twelve(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -12 test/data/short-sample.txt";
    expected_output = "\
line 1\n\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_twelve_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -12 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n\
line 1\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_twelve(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +12 test/data/short-sample.txt";
    expected_output = "\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_twelve_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +12 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_thirteen(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 13 test/data/short-sample.txt";
    expected_output = "\
line 1\n\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_thirteen_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n 13 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n\
line 1\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_thirteen(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -13 test/data/short-sample.txt";
    expected_output = "\
line 1\n\
line 2\n\
line 3\n\
line 4\n\
line 5\n\
line 6\n\
line 7\n\
line 8\n\
line 9\n\
line 10\n\
line 11\n\
line 12\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_negative_thirteen_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n -13 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n\
line 1\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_thirteen(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +13 test/data/short-sample.txt";
    expected_output = "";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

void
test_tail_when_n_is_positive_thirteen_and_r_is_set(void) {
    char *command, *expected_output;
    int expected_status;

    command = "tail -n +13 -r test/data/short-sample.txt";
    expected_output = "\
line 12\n\
line 11\n\
line 10\n\
line 9\n\
line 8\n\
line 7\n\
line 6\n\
line 5\n\
line 4\n\
line 3\n\
line 2\n\
line 1\n";
    expected_status = 0;

    test_assert_tail(expected_status, expected_output, command);
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tail_when_file_does_not_exist);
    RUN_TEST(test_tail_with_default_arguments);
    RUN_TEST(test_tail_when_r_is_set);
    RUN_TEST(test_tail_when_q_is_set_with_a_single_input_file);
    RUN_TEST(test_tail_with_default_arguments_and_multiple_input_files);
    RUN_TEST(test_tail_when_q_is_set_with_multiple_input_files);
    RUN_TEST(test_tail_when_n_is_zero);
    RUN_TEST(test_tail_when_n_is_zero_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_negative_zero);
    RUN_TEST(test_tail_when_n_is_negative_zero_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_positive_zero);
    RUN_TEST(test_tail_when_n_is_positive_zero_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_one);
    RUN_TEST(test_tail_when_n_is_one_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_negative_one);
    RUN_TEST(test_tail_when_n_is_negative_one_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_positive_one);
    RUN_TEST(test_tail_when_n_is_positive_one_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_two);
    RUN_TEST(test_tail_when_n_is_two_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_negative_two);
    RUN_TEST(test_tail_when_n_is_negative_two_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_positive_two);
    RUN_TEST(test_tail_when_n_is_positive_two_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_eleven);
    RUN_TEST(test_tail_when_n_is_eleven_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_negative_eleven);
    RUN_TEST(test_tail_when_n_is_negative_eleven_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_positive_eleven);
    RUN_TEST(test_tail_when_n_is_positive_eleven_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_twelve);
    RUN_TEST(test_tail_when_n_is_twelve_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_negative_twelve);
    RUN_TEST(test_tail_when_n_is_negative_twelve_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_positive_twelve);
    RUN_TEST(test_tail_when_n_is_positive_twelve_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_thirteen);
    RUN_TEST(test_tail_when_n_is_thirteen_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_negative_thirteen);
    RUN_TEST(test_tail_when_n_is_negative_thirteen_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_positive_thirteen);
    RUN_TEST(test_tail_when_n_is_positive_thirteen_and_r_is_set);
    return UNITY_END();
}
