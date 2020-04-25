#include "unity.h"
#include "helpers.h"

void
test_helpers_fprintf_gracefully_handles_a_null_fp(void) {
    FILE *fp, *output;
    long fromOffset, toOffset;

    fromOffset = 10L;
    toOffset = 20L;
    output = fopen("/tmp/helper_test", "w+");

    fp = NULL;
    helpers_fprintf(fp, output, fromOffset, toOffset);

    fseek(output, 0L, SEEK_END);
    TEST_ASSERT_EQUAL(0, ftell(output));

    fclose(output);
}

void
test_helpers_fprintf_gracefully_handles_a_null_output(void) {
    FILE *fp, *output;
    long fromOffset, toOffset;

    fp = fopen("test/data/short-sample.txt", "r");
    fromOffset = 10L;
    toOffset = 20L;

    output = NULL;
    helpers_fprintf(fp, output, fromOffset, toOffset);

    fclose(fp);
}

void
test_helpers_fprintf_gracefully_handles_out_of_bounds(void) {
    FILE *fp, *output;
    long fromOffset, toOffset;

    fp = fopen("test/data/short-sample.txt", "r");
    output = fopen("/tmp/helper_test", "w+");

    /* Set the starting offset beyond the length of the file */
    fseek(fp, 0L, SEEK_END);
    fromOffset = ftell(fp) + 1;
    toOffset = fromOffset + 10;
    helpers_fprintf(fp, output, fromOffset, toOffset);

    fseek(output, 0L, SEEK_END);
    TEST_ASSERT_EQUAL(0, ftell(output));

    /* Set the ending offset beyond the length of the file */
    fseek(fp, 0L, SEEK_END);
    toOffset = ftell(fp) + 1;
    fromOffset = 0L;
    helpers_fprintf(fp, output, fromOffset, toOffset);

    fseek(output, 0L, SEEK_END);
    TEST_ASSERT_EQUAL(0, ftell(output));

    fclose(fp);
    fclose(output);
}

void
test_helpers_fprintf_gracefully_handles_toOffset_being_greater_than_fromOffset(void) {
    FILE *fp, *output;
    long fromOffset, toOffset;

    fp = fopen("test/data/short-sample.txt", "r");
    output = fopen("/tmp/helper_test", "w+");

    /* fromOffset > toOffset */
    fseek(fp, 0L, SEEK_END);
    fromOffset = 26;
    toOffset = 13;
    helpers_fprintf(fp, output, fromOffset, toOffset);

    fseek(output, 0L, SEEK_END);
    TEST_ASSERT_EQUAL(0, ftell(output));

    /* fromOffset == toOffset */
    fseek(fp, 0L, SEEK_END);
    fromOffset = toOffset = 13;
    helpers_fprintf(fp, output, fromOffset, toOffset);

    fseek(output, 0L, SEEK_END);
    TEST_ASSERT_EQUAL(0, ftell(output));

    fclose(fp);
    fclose(output);
}

void
test_helpers_fprintf_copies_substring(void) {
    char *copy, *expected_copy;
    FILE *fp, *output;
    long fromOffset, toOffset;

    fp = fopen("test/data/short-sample.txt", "r");
    output = fopen("/tmp/helper_test", "w+");

    fseek(fp, 0L, SEEK_END);
    fromOffset = 13;
    toOffset = 26;
    helpers_fprintf(fp, output, fromOffset, toOffset);

    fseek(output, 0L, SEEK_END);
    TEST_ASSERT_EQUAL(13, ftell(output));

    expected_copy = "\n\
line 3\n\
line \
";
    copy = (char *) calloc(toOffset - fromOffset + 1, sizeof(char));
    rewind(output);
    fread(copy, sizeof(char), toOffset - fromOffset, output);
    TEST_ASSERT_EQUAL_STRING(expected_copy, copy);
    free(copy);

    fclose(fp);
    fclose(output);
}

void
test_helpers_set_position_gracefully_handles_a_null_fp(void) {
    bool rFlag;
    direction_t nDirection;
    FILE *fp;
    long nCharacters;

    rFlag = false;
    fp = NULL;
    nCharacters = 0L;
    nDirection = RELATIVE_TO_END;

    helpers_set_position(fp, nCharacters, nDirection, rFlag);
}

void
test_helpers_set_position_when_file_is_empty(void) {
    bool rFlag;
    FILE *fp;
    long nCharacters;

    rFlag = false;
    fp = fopen("test/data/empty-file.txt", "r");
    nCharacters = 13;

    helpers_set_position(fp, nCharacters, RELATIVE_TO_BEGINNING, rFlag);
    TEST_ASSERT_EQUAL(0L, ftell(fp));

    helpers_set_position(fp, nCharacters, RELATIVE_TO_END, rFlag);
    TEST_ASSERT_EQUAL(0L, ftell(fp));

    fclose(fp);
}

void
test_helpers_set_position_when_requested_position_is_beyond_file_length(void) {
    bool rFlag;
    FILE *fp;
    long nCharacters;

    rFlag = false;
    fp = fopen("test/data/short-sample.txt", "r");
    nCharacters = 100;

    helpers_set_position(fp, nCharacters, RELATIVE_TO_BEGINNING, rFlag);
    TEST_ASSERT_EQUAL(87, ftell(fp));

    helpers_set_position(fp, nCharacters, RELATIVE_TO_END, rFlag);
    TEST_ASSERT_EQUAL(0L, ftell(fp));

    fclose(fp);
}

void
test_helpers_set_position_when_requested_position_is_within_file_range(void) {
    FILE *fp;
    long nCharacters;

    fp = fopen("test/data/short-sample.txt", "r");
    nCharacters = 13;

    /*
     * NOTE: `tail -r -n +nValue` behaves in a similar manner as the
     *       command without the leading plus. The result reverses the
     *       last nValue lines from the end of the file.
     */
    helpers_set_position(fp, nCharacters, RELATIVE_TO_BEGINNING, true);
    TEST_ASSERT_EQUAL(87 - 13, ftell(fp));

    /*
     * NOTE: `tail -n +nValue` is different in that it begins at nValue
     *       lines from the beginning of the file.
     */
    helpers_set_position(fp, nCharacters, RELATIVE_TO_BEGINNING, false);
    TEST_ASSERT_EQUAL(13, ftell(fp));

    helpers_set_position(fp, nCharacters, RELATIVE_TO_END, true);
    TEST_ASSERT_EQUAL(87 - 13, ftell(fp));

    helpers_set_position(fp, nCharacters, RELATIVE_TO_END, false);
    TEST_ASSERT_EQUAL(87 - 13, ftell(fp));

    fclose(fp);
}

int
main (void) {
    UNITY_BEGIN();
    RUN_TEST(test_helpers_fprintf_gracefully_handles_a_null_fp);
    RUN_TEST(test_helpers_fprintf_gracefully_handles_a_null_output);
    RUN_TEST(test_helpers_fprintf_gracefully_handles_out_of_bounds);
    RUN_TEST(test_helpers_fprintf_gracefully_handles_toOffset_being_greater_than_fromOffset);
    RUN_TEST(test_helpers_fprintf_copies_substring);
    RUN_TEST(test_helpers_set_position_gracefully_handles_a_null_fp);
    RUN_TEST(test_helpers_set_position_when_file_is_empty);
    RUN_TEST(test_helpers_set_position_when_requested_position_is_beyond_file_length);
    RUN_TEST(test_helpers_set_position_when_requested_position_is_within_file_range);
    return UNITY_END();
}
