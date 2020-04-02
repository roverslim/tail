#include "tail_n.h"

#include <time.h>
#include "unity.h"

void
test_assert_tail(
      int expectedStatus, char *expectedOutput,
      int argc, char **argv
) {
    char *output, testFilename[255];
    FILE *fp;
    int status;

    output = (char *) calloc(strlen(expectedOutput) + 1, sizeof(char));
    TEST_ASSERT_NOT_NULL(output);

    srand(time(0));
    sprintf(testFilename, "/tmp/tail_test.%d", rand());
    fp = fopen(testFilename, "w+");
    TEST_ASSERT_NOT_NULL(fp);

    status = tail(argc, argv, fp);

    fseek(fp, 0L, SEEK_SET);
    fread(output, sizeof(char), strlen(expectedOutput) + 1, fp);
    fclose(fp);
    remove(testFilename);

    TEST_ASSERT_EQUAL(expectedStatus, status);
    TEST_ASSERT_EQUAL_STRING(expectedOutput, output);
    free(output);
}

void
test_tail_when_file_does_not_exist(void) {
    char *argv0 = "tail";
    char *argv1 = "no-such-file";
    char *argv[] = {argv0, argv1};
    int argc = 2;

    char *expectedOutput;

    expectedOutput = "tail: no-such-file: No such file or directory\n";
    test_assert_tail(-1, expectedOutput, argc, argv);
}

void
test_tail_with_default_arguments(void) {
    char *argv0 = "tail";
    char *argv1 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1};
    int argc = 2;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_r_is_set(void) {
    char *argv0 = "tail";
    char *argv1 = "-r";
    char *argv2 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2};
    int argc = 3;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_zero(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "0";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_zero_and_r_is_set(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "0";
    char *argv3 = "-r";
    char *argv4 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3, argv4};
    int argc = 5;

    char *expectedOutput;

    expectedOutput = "";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_negative_zero(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "-0";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_negative_zero_and_r_is_set(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "-0";
    char *argv3 = "-r";
    char *argv4 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3, argv4};
    int argc = 5;

    char *expectedOutput;

    expectedOutput = "";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_positive_zero(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+0";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_positive_zero_and_r_is_set(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+0";
    char *argv3 = "-r";
    char *argv4 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3, argv4};
    int argc = 5;

    char *expectedOutput;

    expectedOutput = "";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_one(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "1";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "line 12\n";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_negative_one(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "-1";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "line 12\n";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_positive_one(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+1";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_two(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "2";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
line 11\n\
line 12\n";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_negative_two(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "-2";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
line 11\n\
line 12\n";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_positive_two(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+2";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_eleven(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "11";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_negative_eleven(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "-11";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_positive_eleven(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+11";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
line 11\n\
line 12\n";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_twelve(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "12";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_negative_twelve(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "-12";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_positive_twelve(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+12";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
line 12\n";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_thirteen(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "13";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_negative_thirteen(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "-13";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "\
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

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_when_n_is_positive_thirteen(void) {
    char *argv0 = "tail";
    char *argv1 = "-n";
    char *argv2 = "+13";
    char *argv3 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1, argv2, argv3};
    int argc = 4;

    char *expectedOutput;

    expectedOutput = "";

    test_assert_tail(0, expectedOutput, argc, argv);
}

void
test_tail_n_when_fp_is_null(void) {
    int exitCode;

    exitCode = tail_n(NULL, true, 13, RELATIVE_TO_END);
    TEST_ASSERT_EQUAL_INT(1, exitCode);

    exitCode = tail_n(NULL, true, 13, RELATIVE_TO_BEGINNING);
    TEST_ASSERT_EQUAL_INT(1, exitCode);
}

void
test_tail_n_when_n_is_zero(void) {
    FILE *fp;
    int exitCode;
    long filePosition;
    unsigned int numLines;

    numLines = 0;

    // tail -n 0 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(87, filePosition);

    // tail -n +0 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);
}

void
test_tail_n_when_n_is_one(void) {
    FILE *fp;
    int exitCode;
    long filePosition;
    unsigned int numLines;

    numLines = 1;

    // tail -n 1 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(79, filePosition);

    // tail -n +1 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);
}

void
test_tail_n_when_n_is_less_than_the_number_of_lines(void) {
    FILE* fp;
    int exitCode;
    long filePosition;
    unsigned int numLines;

    numLines = 3;

    // tail -n 3 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(63, filePosition);

    // tail -n +3 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(14, filePosition);
}

void
test_tail_n_when_n_is_equal_to_the_number_of_lines(void) {
    FILE* fp;
    int exitCode;
    long filePosition;
    unsigned int numLines;

    numLines = 12;

    // tail -n 12 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n +12 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(79, filePosition);
}

void
test_tail_n_when_n_is_more_than_the_number_of_lines(void) {
    FILE* fp;
    int exitCode;
    long filePosition;
    unsigned int numLines;

    numLines = 100;

    // tail -n 100 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n +100 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(87, filePosition);
}

void
test_tail_n_when_input_file_is_empty(void) {
    FILE* fp;
    int exitCode;
    long filePosition;

    // tail test/data/empty-file.txt
    fp = fopen("test/data/empty-file.txt", "r");
    exitCode = tail_n(fp, false, 10, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n 3 test/data/empty-file.txt
    fp = fopen("test/data/empty-file.txt", "r");
    exitCode = tail_n(fp, true, 3, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n +3 test/data/empty-file.txt
    fp = fopen("test/data/empty-file.txt", "r");
    exitCode = tail_n(fp, true, 3, RELATIVE_TO_BEGINNING);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);
}

void
test_tail_n_when_n_is_not_specified(void) {
    FILE* fp;
    int exitCode;
    long filePosition;

    // tail test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, false, 10, RELATIVE_TO_END);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(14, filePosition);
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tail_when_file_does_not_exist);
    RUN_TEST(test_tail_with_default_arguments);
    RUN_TEST(test_tail_when_r_is_set);
    RUN_TEST(test_tail_when_n_is_zero);
    RUN_TEST(test_tail_when_n_is_zero_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_negative_zero);
    RUN_TEST(test_tail_when_n_is_negative_zero_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_positive_zero);
    RUN_TEST(test_tail_when_n_is_positive_zero_and_r_is_set);
    RUN_TEST(test_tail_when_n_is_one);
    RUN_TEST(test_tail_when_n_is_negative_one);
    RUN_TEST(test_tail_when_n_is_positive_one);
    RUN_TEST(test_tail_when_n_is_two);
    RUN_TEST(test_tail_when_n_is_negative_two);
    RUN_TEST(test_tail_when_n_is_positive_two);
    RUN_TEST(test_tail_when_n_is_eleven);
    RUN_TEST(test_tail_when_n_is_negative_eleven);
    RUN_TEST(test_tail_when_n_is_positive_eleven);
    RUN_TEST(test_tail_when_n_is_twelve);
    RUN_TEST(test_tail_when_n_is_negative_twelve);
    RUN_TEST(test_tail_when_n_is_positive_twelve);
    RUN_TEST(test_tail_when_n_is_thirteen);
    RUN_TEST(test_tail_when_n_is_negative_thirteen);
    RUN_TEST(test_tail_when_n_is_positive_thirteen);
    RUN_TEST(test_tail_n_when_fp_is_null);
    RUN_TEST(test_tail_n_when_n_is_zero);
    RUN_TEST(test_tail_n_when_n_is_one);
    RUN_TEST(test_tail_n_when_n_is_less_than_the_number_of_lines);
    RUN_TEST(test_tail_n_when_n_is_equal_to_the_number_of_lines);
    RUN_TEST(test_tail_n_when_n_is_more_than_the_number_of_lines);
    RUN_TEST(test_tail_n_when_input_file_is_empty);
    RUN_TEST(test_tail_n_when_n_is_not_specified);
    return UNITY_END();
}
