#include "unity.h"
#include "circular_buffer_variants.h"

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

void setUp(void) {}
void tearDown(void) {}


void test_f32_cb_init_and_push_single(void) {
    float buf_storage[4];
    f32_cb_t cb;
    float val;

    TEST_CB_OK(f32_cb_init(&cb, buf_storage, 4));

    f32_cb_push(&cb, 10.1f);
    TEST_CB_OK(f32_cb_get_delayed(&cb, 0, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 10.1f, val);
}

void test_f32_cb_init_and_push(void) {
    float buf_storage[4];
    f32_cb_t cb;
    float val;

    TEST_CB_OK(f32_cb_init(&cb, buf_storage, 4));

    f32_cb_push(&cb, 10.1f);
    f32_cb_push(&cb, 20.2f);
    f32_cb_push(&cb, 30.3f);
    f32_cb_push(&cb, 40.5f);

    TEST_CB_OK(f32_cb_get_delayed(&cb, 0, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 40.5f, val);

    TEST_CB_OK(f32_cb_get_delayed(&cb, 1, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 30.3f, val);

    TEST_CB_OK(f32_cb_get_delayed(&cb, 2, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 20.2f, val);

    TEST_CB_OK(f32_cb_get_delayed(&cb, 3, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 10.1f, val);
}


void test_f32_cb_init_error(void) {
    f32_cb_t cb;
    float buf_storage[4];

    TEST_CB_STATUS(f32_cb_init(&cb, NULL, 4), cb_status_error_null);

    TEST_CB_STATUS(f32_cb_init(NULL, buf_storage, 4), cb_status_error_null);
}

void test_f32_cb_wraparound_behavior(void) {
    float buf_storage[3];
    f32_cb_t cb;
    float val;

    f32_cb_init(&cb, buf_storage, 3);

    f32_cb_push(&cb, 1.0f);
    f32_cb_push(&cb, 2.0f);
    f32_cb_push(&cb, 3.0f);
    f32_cb_push(&cb, 4.0f);  // overwrites 1
    f32_cb_push(&cb, 5.0f);  // overwrites 2

    TEST_CB_OK(f32_cb_get_delayed(&cb, 0, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 5.0f, val);

    TEST_CB_OK(f32_cb_get_delayed(&cb, 1, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 4.0f, val);

    TEST_CB_OK(f32_cb_get_delayed(&cb, 2, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 3.0f, val);
}

void test_f32_cb_delay_too_large(void) {
    float buf_storage[3];
    f32_cb_t cb;
    float val;

    f32_cb_init(&cb, buf_storage, 3);
    f32_cb_push(&cb, 1.0f);
    f32_cb_push(&cb, 2.0f);
    f32_cb_push(&cb, 3.0f);

    TEST_CB_STATUS(f32_cb_get_delayed(&cb, 3, &val), cb_status_error_delay_too_large);
    TEST_CB_STATUS(f32_cb_get_delayed(&cb, 100, &val), cb_status_error_delay_too_large);
    TEST_CB_STATUS(f32_cb_get_delayed(&cb, 1, NULL), cb_status_error_null);
}

void test_u8_cb_version(void) {
    uint8_t buf[2];
    u8_cb_t cb;
    uint8_t val;

    u8_cb_init(&cb, buf, 2);

    u8_cb_push(&cb, 1);
    u8_cb_push(&cb, 2);
    u8_cb_push(&cb, 3);

    TEST_CB_OK(u8_cb_get_delayed(&cb, 0, &val));
    TEST_ASSERT_EQUAL_UINT8(3, val);

    TEST_CB_OK(u8_cb_get_delayed(&cb, 1, &val));
    TEST_ASSERT_EQUAL_UINT8(2, val);

    TEST_CB_STATUS(u8_cb_get_delayed(&cb, 2, &val), cb_status_error_delay_too_large);
}

void test_f32_cb_get_count(void) {
    float buf_storage[4];
    f32_cb_t cb;

    TEST_CB_OK(f32_cb_init(&cb, buf_storage, 4));
    TEST_ASSERT_EQUAL_UINT32(0, f32_cb_get_count(&cb));  // empty

    f32_cb_push(&cb, 1.0f);
    TEST_ASSERT_EQUAL_UINT32(1, f32_cb_get_count(&cb));

    f32_cb_push(&cb, 2.0f);
    f32_cb_push(&cb, 3.0f);
    TEST_ASSERT_EQUAL_UINT32(3, f32_cb_get_count(&cb));

    f32_cb_push(&cb, 4.0f);
    TEST_ASSERT_EQUAL_UINT32(4, f32_cb_get_count(&cb));  // full

    f32_cb_push(&cb, 5.0f);  // overwrite oldest
    TEST_ASSERT_EQUAL_UINT32(4, f32_cb_get_count(&cb));  // still full
}

void test_f32_cb_get_delayed_on_empty(void) {
    float buf_storage[4];
    f32_cb_t cb;
    float val;

    TEST_CB_OK(f32_cb_init(&cb, buf_storage, 4));
    TEST_CB_STATUS(f32_cb_get_delayed(&cb, 0, &val), cb_status_error_delay_too_large);
    TEST_CB_STATUS(f32_cb_get_delayed(&cb, 1, &val), cb_status_error_delay_too_large);
}

void test_f32_cb_get_delayed_partial_buffer(void) {
    float buf_storage[4];
    f32_cb_t cb;
    float val;

    f32_cb_init(&cb, buf_storage, 4);
    f32_cb_push(&cb, 1.0f);
    f32_cb_push(&cb, 2.0f);  // count = 2

    TEST_CB_OK(f32_cb_get_delayed(&cb, 0, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2.0f, val);

    TEST_CB_OK(f32_cb_get_delayed(&cb, 1, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0f, val);

    TEST_CB_STATUS(f32_cb_get_delayed(&cb, 2, &val), cb_status_error_delay_too_large);
}

void test_f32_cb_delay_equal_to_capacity_should_fail(void) {
    float buf_storage[4];
    f32_cb_t cb;
    float val;

    TEST_CB_OK(f32_cb_init(&cb, buf_storage, 4));

    // Fill the buffer to its full capacity
    f32_cb_push(&cb, 1.0f);
    f32_cb_push(&cb, 2.0f);
    f32_cb_push(&cb, 3.0f);
    f32_cb_push(&cb, 4.0f);  // Now cb.count == 4

    // Attempting to access delay equal to capacity (i.e., 4) should fail
    TEST_CB_STATUS(f32_cb_get_delayed(&cb, 4, &val), cb_status_error_delay_too_large);
}

void test_f32_cb_delay_max_valid_when_full(void) {
    float buf_storage[4];
    f32_cb_t cb;
    float val;

    TEST_CB_OK(f32_cb_init(&cb, buf_storage, 4));

    // Fill the buffer with known values
    f32_cb_push(&cb, 1.0f); // oldest
    f32_cb_push(&cb, 2.0f);
    f32_cb_push(&cb, 3.0f);
    f32_cb_push(&cb, 4.0f); // newest

    // Request delay == count - 1 == 3
    TEST_CB_OK(f32_cb_get_delayed(&cb, 3, &val));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0f, val);  // Should return the oldest value
}
