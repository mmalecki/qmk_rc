/*
 * Lifted from libuv's `test/task.h` and generalized.
 * Thanks for the best C testing framework out there, libuv contributors!
 */

/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef TASK_H_
#define TASK_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* Die with fatal error. */
#define FATAL(msg)                                        \
  do {                                                    \
    fprintf(stderr,                                       \
            "Fatal error in %s on line %d: %s\n",         \
            __FILE__,                                     \
            __LINE__,                                     \
            msg);                                         \
    fflush(stderr);                                       \
    abort();                                              \
  } while (0)

/* Have our own assert, so we are sure it does not get optimized away in
 * a release build.
 */
#define ASSERT(expr)                                      \
 do {                                                     \
  if (!(expr)) {                                          \
    fprintf(stderr,                                       \
            "Assertion failed in %s on line %d: %s\n",    \
            __FILE__,                                     \
            __LINE__,                                     \
            #expr);                                       \
    abort();                                              \
  }                                                       \
 } while (0)

#define ASSERT_BASE(a, operator, b, type, conv)              \
 do {                                                        \
  type eval_a = (type) (a);                                  \
  type eval_b = (type) (b);                                  \
  if (!(eval_a operator eval_b)) {                           \
    fprintf(stderr,                                          \
            "Assertion failed in %s on line %d: `%s %s %s` " \
            "(%"conv" %s %"conv")\n",                        \
            __FILE__,                                        \
            __LINE__,                                        \
            #a,                                              \
            #operator,                                       \
            #b,                                              \
            eval_a,                                          \
            #operator,                                       \
            eval_b);                                         \
    abort();                                                 \
  }                                                          \
 } while (0)

#define ASSERT_BASE_STR(expr, a, operator, b, type, conv)      \
 do {                                                          \
  if (!(expr)) {                                               \
    fprintf(stderr,                                            \
            "Assertion failed in %s on line %d: `%s %s %s` "   \
            "(%"conv" %s %"conv")\n",                          \
            __FILE__,                                          \
            __LINE__,                                          \
            #a,                                                \
            #operator,                                         \
            #b,                                                \
            (type)a,                                           \
            #operator,                                         \
            (type)b);                                          \
    abort();                                                   \
  }                                                            \
 } while (0)

#define ASSERT_BASE_LEN(expr, a, operator, b, conv, len)     \
 do {                                                        \
  if (!(expr)) {                                             \
    fprintf(stderr,                                          \
            "Assertion failed in %s on line %d: `%s %s %s` " \
            "(%.*"#conv" %s %.*"#conv")\n",                  \
            __FILE__,                                        \
            __LINE__,                                        \
            #a,                                              \
            #operator,                                       \
            #b,                                              \
            (int)len,                                        \
            a,                                               \
            #operator,                                       \
            (int)len,                                        \
            b);                                              \
    abort();                                                 \
  }                                                          \
 } while (0)

#define ASSERT_BASE_HEX(expr, a, operator, b, size)            \
 do {                                                          \
  if (!(expr)) {                                               \
    int i;                                                     \
    unsigned char* a_ = (unsigned char*)a;                     \
    unsigned char* b_ = (unsigned char*)b;                     \
    fprintf(stderr,                                            \
            "Assertion failed in %s on line %d: `%s %s %s` (", \
            __FILE__,                                          \
            __LINE__,                                          \
            #a,                                                \
            #operator,                                         \
            #b);                                               \
    for (i = 0; i < size; ++i) {                               \
      if (i > 0) fprintf(stderr, ":");                         \
      fprintf(stderr, "%02X", a_[i]);                          \
    }                                                          \
    fprintf(stderr, " %s ", #operator);                        \
    for (i = 0; i < size; ++i) {                               \
      if (i > 0) fprintf(stderr, ":");                         \
      fprintf(stderr, "%02X", b_[i]);                          \
    }                                                          \
    fprintf(stderr, ")\n");                                    \
    abort();                                                   \
  }                                                            \
 } while (0)

#define ASSERT_INT_BASE(a, operator, b, type, conv)          \
 ASSERT_BASE(a, operator, b, type, conv)

#define ASSERT_EQ(a, b) ASSERT_INT_BASE(a, ==, b, int64_t, PRId64)
#define ASSERT_GE(a, b) ASSERT_INT_BASE(a, >=, b, int64_t, PRId64)
#define ASSERT_GT(a, b) ASSERT_INT_BASE(a, >, b, int64_t, PRId64)
#define ASSERT_LE(a, b) ASSERT_INT_BASE(a, <=, b, int64_t, PRId64)
#define ASSERT_LT(a, b) ASSERT_INT_BASE(a, <, b, int64_t, PRId64)
#define ASSERT_NE(a, b) ASSERT_INT_BASE(a, !=, b, int64_t, PRId64)

#define ASSERT_UINT64_EQ(a, b) ASSERT_INT_BASE(a, ==, b, uint64_t, PRIu64)
#define ASSERT_UINT64_GE(a, b) ASSERT_INT_BASE(a, >=, b, uint64_t, PRIu64)
#define ASSERT_UINT64_GT(a, b) ASSERT_INT_BASE(a, >, b, uint64_t, PRIu64)
#define ASSERT_UINT64_LE(a, b) ASSERT_INT_BASE(a, <=, b, uint64_t, PRIu64)
#define ASSERT_UINT64_LT(a, b) ASSERT_INT_BASE(a, <, b, uint64_t, PRIu64)
#define ASSERT_UINT64_NE(a, b) ASSERT_INT_BASE(a, !=, b, uint64_t, PRIu64)

#define ASSERT_STR_EQ(a, b) \
  ASSERT_BASE_STR(strcmp(a, b) == 0, a, == , b, char*, "s")

#define ASSERT_STR_NE(a, b) \
  ASSERT_BASE_STR(strcmp(a, b) != 0, a, !=, b, char*, "s")

#define ASSERT_MEM_EQ(a, b, size) \
  ASSERT_BASE_LEN(memcmp(a, b, size) == 0, a, ==, b, s, size)

#define ASSERT_MEM_NE(a, b, size) \
  ASSERT_BASE_LEN(memcmp(a, b, size) != 0, a, !=, b, s, size)

#define ASSERT_MEM_HEX_EQ(a, b, size) \
  ASSERT_BASE_HEX(memcmp(a, b, size) == 0, a, ==, b, size)

#define ASSERT_MEM_HEX_NE(a, b, size) \
  ASSERT_BASE_HEX(memcmp(a, b, size) != 0, a, !=, b, size)

#define ASSERT_NULL(a) \
  ASSERT_BASE(a, ==, NULL, void*, "p")

#define ASSERT_NOT_NULL(a) \
  ASSERT_BASE(a, !=, NULL, void*, "p")

#define ASSERT_PTR_EQ(a, b) \
  ASSERT_BASE(a, ==, b, void*, "p")

#define ASSERT_PTR_NE(a, b) \
  ASSERT_BASE(a, !=, b, void*, "p")

/* Just sugar for wrapping the main() for a task or helper. */
#define TEST_IMPL(name)                                                       \
  int run_test_##name(void);                                                  \
  int run_test_##name(void)

#define BENCHMARK_IMPL(name)                                                  \
  int run_benchmark_##name(void);                                             \
  int run_benchmark_##name(void)

#define HELPER_IMPL(name)                                                     \
  int run_helper_##name(void);                                                \
  int run_helper_##name(void)

/* Format big numbers nicely. WARNING: leaks memory. */
const char* fmt(double d);

/* Reserved test exit codes. */
enum test_status {
  TEST_OK = 0,
  TEST_SKIP
};

#define RETURN_OK()                                                           \
  do {                                                                        \
    return TEST_OK;                                                           \
  } while (0)

#define RETURN_SKIP(explanation)                                              \
  do {                                                                        \
    fprintf(stderr, "%s\n", explanation);                                     \
    fflush(stderr);                                                           \
    return TEST_SKIP;                                                         \
  } while (0)

#if defined(__clang__) ||                                \
    defined(__GNUC__) ||                                 \
    defined(__INTEL_COMPILER)
# define UNUSED __attribute__((unused))
#else
# define UNUSED
#endif

#endif /* TASK_H_ */
