#include "unity.h"

// Helper macros for test status checking
#define TEST_CB_OK(expr) \
    do { \
        cb_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(cb_status_ok, status, "Expected cb_status_ok"); \
    } while (0)

#define TEST_CB_STATUS(expr, expected) \
    do { \
        cb_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), status, "Unexpected cb_status_t value"); \
    } while (0)

// uint8_t variant
#define CIRCULAR_BUFFER_TYPE uint8_t
#define CIRCULAR_BUFFER_PREFIX u8
#include "circular_buffer.h"
#undef CIRCULAR_BUFFER_TYPE
#undef CIRCULAR_BUFFER_PREFIX

// float variant
#define CIRCULAR_BUFFER_TYPE float
#define CIRCULAR_BUFFER_PREFIX f32
#include "circular_buffer.h"
#undef CIRCULAR_BUFFER_TYPE
#undef CIRCULAR_BUFFER_PREFIX

void setUp(void) {}
void tearDown(void) {}

void test_u8_init_and_push(void) {
    uint8_t buf_storage[4];
    u8_t cb;
    uint8_t val;

    TEST_CB_OK(u8_init(&cb, buf_storage, 4));

    u8_push(&cb, 10);
    u8_push(&cb, 20);
    u8_push(&cb, 30);
    u8_push(&cb, 40);

    TEST_CB_OK(u8_get_delayed(&cb, 0, &val));
    TEST_ASSERT_EQUAL_UINT8(40, val);

    TEST_CB_OK(u8_get_delayed(&cb, 1, &val));
    TEST_ASSERT_EQUAL_UINT8(30, val);

    TEST_CB_OK(u8_get_delayed(&cb, 2, &val));
    TEST_ASSERT_EQUAL_UINT8(20, val);

    TEST_CB_OK(u8_get_delayed(&cb, 3, &val));
    TEST_ASSERT_EQUAL_UINT8(10, val);
}


void test_u8_init_error(void) {
    u8_t cb;
    uint8_t buf_storage[4];

    TEST_CB_STATUS(u8_init(&cb, NULL, 4), cb_status_error_null);

    TEST_CB_STATUS(u8_init(NULL, buf_storage, 4), cb_status_error_null);
}

void test_u8_wraparound_behavior(void) {
    uint8_t buf_storage[3];
    u8_t cb;
    uint8_t val;

    u8_init(&cb, buf_storage, 3);

    u8_push(&cb, 1);
    u8_push(&cb, 2);
    u8_push(&cb, 3);
    u8_push(&cb, 4);  // overwrites 1
    u8_push(&cb, 5);  // overwrites 2

    TEST_CB_OK(u8_get_delayed(&cb, 0, &val));
    TEST_ASSERT_EQUAL_UINT8(5, val);

    TEST_CB_OK(u8_get_delayed(&cb, 1, &val));
    TEST_ASSERT_EQUAL_UINT8(4, val);

    TEST_CB_OK(u8_get_delayed(&cb, 2, &val));
    TEST_ASSERT_EQUAL_UINT8(3, val);
}

void test_u8_delay_too_large(void) {
    uint8_t buf_storage[3];
    u8_t cb;
    uint8_t val;

    u8_init(&cb, buf_storage, 3);
    u8_push(&cb, 1);
    u8_push(&cb, 2);
    u8_push(&cb, 3);

    TEST_CB_STATUS(u8_get_delayed(&cb, 3, &val), cb_status_error_delay_too_large);
    TEST_CB_STATUS(u8_get_delayed(&cb, 100, &val), cb_status_error_delay_too_large);
    TEST_CB_STATUS(u8_get_delayed(&cb, 1, NULL), cb_status_error_null);
}

void test_f32_float_version(void) {
    float buf[2];
    f32_t cb;
    float val;

    f32_init(&cb, buf, 2);

    f32_push(&cb, 0.5f);
    f32_push(&cb, 1.5f);
    f32_push(&cb, 2.5f);

    TEST_CB_OK(f32_get_delayed(&cb, 0, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.5f, val);

    TEST_CB_OK(f32_get_delayed(&cb, 1, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, val);

    TEST_CB_STATUS(f32_get_delayed(&cb, 2, &val), cb_status_error_delay_too_large);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_u8_init_and_push);
    RUN_TEST(test_u8_init_error);
    RUN_TEST(test_u8_wraparound_behavior);
    RUN_TEST(test_u8_delay_too_large);
    RUN_TEST(test_f32_float_version);
    return UNITY_END();
}

