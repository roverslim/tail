#include "unity.h"
#include "tail_n.h"

void test_tail_n_when_n_is_zero(void) {
    int num_lines, exit_code;

    num_lines = 0;

    exit_code = tail_n(NULL, num_lines);
    TEST_ASSERT_EQUAL_INT(0, exit_code);
}

void test_tail_n_when_n_is_less_than_the_number_of_line_breaks(void) {
    FILE* fp;
    int num_lines, exit_code;
    long file_position;

    num_lines = 6;
    fp = fopen("short-sample.txt", "r");

    exit_code = tail_n(fp, num_lines);
    file_position = ftell(fp);
    TEST_ASSERT_EQUAL_INT(0, exit_code);
    TEST_ASSERT_EQUAL_INT(42, file_position);
}

void test_tail_n_when_n_is_equal_to_the_number_of_line_breaks(void) {
    FILE* fp;
    int num_lines, exit_code;
    long file_position;

    num_lines = 12;
    fp = fopen("short-sample.txt", "r");

    exit_code = tail_n(fp, num_lines);
    file_position = ftell(fp);
    TEST_ASSERT_EQUAL_INT(0, exit_code);
    TEST_ASSERT_EQUAL_INT(1, file_position);
}

void test_tail_n_when_n_is_more_than_the_number_of_line_breaks(void) {
    FILE* fp;
    int num_lines, exit_code;
    long file_position;

    num_lines = 100;
    fp = fopen("short-sample.txt", "r");

    exit_code = tail_n(fp, num_lines);
    file_position = ftell(fp);
    TEST_ASSERT_EQUAL_INT(0, exit_code);
    TEST_ASSERT_EQUAL_INT(1, file_position);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tail_n_when_n_is_zero);
    RUN_TEST(test_tail_n_when_n_is_less_than_the_number_of_line_breaks);
    RUN_TEST(test_tail_n_when_n_is_equal_to_the_number_of_line_breaks);
    RUN_TEST(test_tail_n_when_n_is_more_than_the_number_of_line_breaks);
    return UNITY_END();
}
