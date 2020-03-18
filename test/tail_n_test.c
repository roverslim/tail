#include "tail_n.h"

#include <time.h>
#include "unity.h"

static void
write_buffer(FILE *fp, char **buffer) {
    int c, i = 0;

    fseek(fp, 0L, SEEK_SET);
    while ((c = fgetc(fp)) != EOF) {
        sprintf((*buffer + i), "%c", c);
        i++;
    }
}

void
test_tail_when_file_does_not_exist(void) {
    char *argv0 = "tail";
    char *argv1 = "no-such-file";
    char *argv[] = {argv0, argv1};
    int argc = 2, status;

    char *output, *expectedOutput, testFilename[255];
    FILE *fp;

    expectedOutput = "tail: no-such-file: No such file or directory\n";
    output = malloc(strlen(expectedOutput) + 1);

    srand(time(0));
    sprintf(testFilename, "/tmp/tail_test.%d", rand());
    fp = fopen(testFilename, "w+");
    TEST_ASSERT_NOT_NULL(fp);

    status = tail(argc, argv, fp);

    write_buffer(fp, &output);
    fclose(fp);

    TEST_ASSERT_EQUAL(-1, status);
    TEST_ASSERT_EQUAL_STRING(expectedOutput, output);
    free(output);
}

void
test_tail_with_default_arguments(void) {
    char *argv0 = "tail";
    char *argv1 = "test/data/short-sample.txt";
    char *argv[] = {argv0, argv1};
    int argc = 2, status;

    char *output, *expectedOutput, testFilename[255];
    FILE *fp;

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
    output = malloc(strlen(expectedOutput) + 1);

    srand(time(0));
    sprintf(testFilename, "/tmp/tail_test.%d", rand());
    fp = fopen(testFilename, "w+");
    TEST_ASSERT_NOT_NULL(fp);

    status = tail(argc, argv, fp);

    write_buffer(fp, &output);
    fclose(fp);

    TEST_ASSERT_EQUAL(0, status);
    TEST_ASSERT_EQUAL_STRING(expectedOutput, output);
    free(output);
}

void
test_tail_n_when_fp_is_null(void) {
    int exitCode;

    exitCode = tail_n(NULL, true, 13, RELATIVE_TO_END, 0);
    TEST_ASSERT_EQUAL_INT(1, exitCode);

    exitCode = tail_n(NULL, true, 13, RELATIVE_TO_BEGINNING, 0);
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
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(87, filePosition);

    // tail -n +0 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING, 0);
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
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(79, filePosition);

    // tail -n +1 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING, 0);
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
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(63, filePosition);

    // tail -n +3 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING, 0);
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
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n +12 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING, 0);
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
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n +100 test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, true, numLines, RELATIVE_TO_BEGINNING, 0);
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
    exitCode = tail_n(fp, false, 10, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n 3 test/data/empty-file.txt
    fp = fopen("test/data/empty-file.txt", "r");
    exitCode = tail_n(fp, true, 3, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -n +3 test/data/empty-file.txt
    fp = fopen("test/data/empty-file.txt", "r");
    exitCode = tail_n(fp, true, 3, RELATIVE_TO_BEGINNING, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);

    // tail -r test/data/empty-file.txt
    fp = fopen("test/data/empty-file.txt", "r");
    exitCode = tail_n(fp, false, 10, RELATIVE_TO_END, 1);
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
    exitCode = tail_n(fp, false, 10, RELATIVE_TO_END, 0);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(14, filePosition);

    // tail -r test/data/short-sample.txt
    fp = fopen("test/data/short-sample.txt", "r");
    exitCode = tail_n(fp, false, 10, RELATIVE_TO_END, 1);
    filePosition = ftell(fp);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(0, exitCode);
    TEST_ASSERT_EQUAL_INT(0, filePosition);
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tail_when_file_does_not_exist);
    RUN_TEST(test_tail_with_default_arguments);
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
