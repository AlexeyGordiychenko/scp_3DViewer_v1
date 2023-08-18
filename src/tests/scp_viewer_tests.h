#ifndef SCP_VIEWER_TESTS_H
#define SCP_VIEWER_TESTS_H

#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../scp_viewer.h"

// #define MAX_EXP_LEN 255
#define PRE_TEST_HEADER "\033[33m**"
#define POST_TEST_HEADER "**\033[0m"

Suite *scp_viewer_tests(void);

#endif  // SCP_VIEWER_TESTS_H
