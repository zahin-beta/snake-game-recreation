#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int unity_pass_count  = 0;
static int unity_fail_count  = 0;
static int unity_current_fails = 0;

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { \
        int _e = (expected); \
        int _a = (actual); \
        if (_e != _a) { \
            unity_current_fails++; \
            printf("FAIL %s:%d: expected %d but got %d\n", __FILE__, __LINE__, _e, _a); \
        } \
    } while(0)

#define TEST_ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            unity_current_fails++; \
            printf("FAIL %s:%d: expected true\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_FALSE(condition) \
    do { \
        if ((condition)) { \
            unity_current_fails++; \
            printf("FAIL %s:%d: expected false\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define RUN_TEST(func) \
    do { \
        unity_current_fails = 0; \
        func(); \
        if (unity_current_fails == 0) { \
            unity_pass_count++; \
            printf("PASS  %s\n", #func); \
        } else { \
            unity_fail_count += unity_current_fails; \
            printf("FAIL  %s (%d asserts failed)\n", #func, unity_current_fails); \
        } \
    } while(0)

#define UNITY_END() \
    do { \
        printf("--------------------\n"); \
        printf("%d passed, %d failed out of %d tests\n", \
               unity_pass_count, unity_fail_count, unity_pass_count + unity_fail_count); \
        exit(unity_fail_count > 0 ? 1 : 0); \
    } while(0)

#endif
