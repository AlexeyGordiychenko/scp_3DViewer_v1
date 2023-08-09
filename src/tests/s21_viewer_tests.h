#ifndef S21_VIEWER_TESTS_H
#define S21_VIEWER_TESTS_H

#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_viewer.h"

// #define MAX_EXP_LEN 255
#define PRE_TEST_HEADER "\033[33m**"
#define POST_TEST_HEADER "**\033[0m"

Suite *s21_viewer_tests(void);

#endif  // S21_VIEWER_TESTS_H
