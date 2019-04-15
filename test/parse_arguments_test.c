#include "unity.h"
#include "parse_arguments.h"

void
test_n_value_gets_set(void) {
    arguments_t *arguments;

    int argc = 2;
    char arg0[] = "tail";
    char arg1[] = "-n3";
    char *argv[] = {(char *)&arg0, (char *)&arg1};

    arguments = parse_arguments(argc, argv);
    TEST_ASSERT_EQUAL(3, arguments_get_n(arguments));
}

int
main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_n_value_gets_set);
    return UNITY_END();
}
