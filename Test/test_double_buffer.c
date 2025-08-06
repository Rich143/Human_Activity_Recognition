#include "unity.h"
#include "double_buffer.h"

#include "mock_cmsis_gcc.h"

#include <string.h>
#include <stdlib.h>

// Test instance
static double_buffer_state_t state;

void mock_cmsis_gcc() {
    __get_PRIMASK_IgnoreAndReturn(0);
    __disable_irq_Ignore();
    __DMB_Ignore();
    __set_PRIMASK_Ignore();
}

void setUp(void) {
    mock_cmsis_gcc();

    dbuf_init(&state);
}

void tearDown(void) {
    // Nothing needed
}

void test_initial_state(void) {
    TEST_ASSERT_EQUAL_UINT8(0, dbuf_get_isr_index(&state));
    TEST_ASSERT_EQUAL_INT8(NO_BUFFER_READY, dbuf_get_ready(&state));
}

void test_single_buffer_ready_flow(void) {
    // Initially buffer 0
    TEST_ASSERT_EQUAL_UINT8(0, dbuf_get_isr_index(&state));

    bool success = dbuf_mark_ready(&state);
    TEST_ASSERT_TRUE(success);

    // After marking ready, should now be writing to buffer 1
    TEST_ASSERT_EQUAL_UINT8(1, dbuf_get_isr_index(&state));

    // Main sees buffer 0 is ready
    int8_t ready = dbuf_get_ready(&state);
    TEST_ASSERT_EQUAL_INT8(0, ready);

    // Main processes the buffer
    dbuf_mark_processed(&state);
    TEST_ASSERT_EQUAL_INT8(NO_BUFFER_READY, dbuf_get_ready(&state));
}

void test_second_mark_ready_blocks_until_processed(void) {
    // Mark first buffer ready
    TEST_ASSERT_TRUE(dbuf_mark_ready(&state));
    TEST_ASSERT_EQUAL_INT8(0, dbuf_get_ready(&state));

    // Try to mark another before main processes it — should fail
    TEST_ASSERT_FALSE(dbuf_mark_ready(&state));

    // Process the first
    dbuf_mark_processed(&state);
    TEST_ASSERT_EQUAL_INT8(NO_BUFFER_READY, dbuf_get_ready(&state));

    // Now second one should succeed
    TEST_ASSERT_TRUE(dbuf_mark_ready(&state));
    TEST_ASSERT_EQUAL_INT8(1, dbuf_get_ready(&state));
    TEST_ASSERT_EQUAL_UINT8(0, dbuf_get_isr_index(&state));
}

void test_toggle_isr_index(void) {
    TEST_ASSERT_EQUAL_UINT8(0, dbuf_get_isr_index(&state));

    TEST_ASSERT_TRUE(dbuf_mark_ready(&state));       // ISR completes buffer 0
    TEST_ASSERT_EQUAL_UINT8(1, dbuf_get_isr_index(&state));

    TEST_ASSERT_FALSE(dbuf_mark_ready(&state));      // Cannot mark buffer 1 yet
    TEST_ASSERT_EQUAL_UINT8(1, dbuf_get_isr_index(&state));  // Still buffer 1

    dbuf_mark_processed(&state);                     // Main processes buffer 0

    TEST_ASSERT_TRUE(dbuf_mark_ready(&state));       // Now buffer 1 can be marked ready
    TEST_ASSERT_EQUAL_UINT8(0, dbuf_get_isr_index(&state));
}

void test_double_ready_should_not_overwrite(void) {
    dbuf_init(&state);

    // ISR marks buffer 0 as ready
    TEST_ASSERT_TRUE(dbuf_mark_ready(&state));
    TEST_ASSERT_EQUAL_INT8(0, dbuf_get_ready(&state));

    // ISR tries to mark buffer 1 before main processes buffer 0
    TEST_ASSERT_FALSE(dbuf_mark_ready(&state));

    // Main should still see buffer 0 as ready
    TEST_ASSERT_EQUAL_INT8(0, dbuf_get_ready(&state));
}

void test_init_resets_state(void) {
    dbuf_init(&state);
    TEST_ASSERT_TRUE(dbuf_mark_ready(&state));  // mark buffer 0

    dbuf_init(&state);  // Reset state

    TEST_ASSERT_EQUAL_UINT8(0, dbuf_get_isr_index(&state));
    TEST_ASSERT_EQUAL_INT8(NO_BUFFER_READY, dbuf_get_ready(&state));
}
void test_get_ready_is_stable_until_processed(void) {
    dbuf_init(&state);

    TEST_ASSERT_TRUE(dbuf_mark_ready(&state));  // buffer 0 ready

    // Call get_ready multiple times before processing
    TEST_ASSERT_EQUAL_INT8(0, dbuf_get_ready(&state));
    TEST_ASSERT_EQUAL_INT8(0, dbuf_get_ready(&state));
    TEST_ASSERT_EQUAL_INT8(0, dbuf_get_ready(&state));

    // Now process it
    dbuf_mark_processed(&state);
    TEST_ASSERT_EQUAL_INT8(NO_BUFFER_READY, dbuf_get_ready(&state));
}


#include <pthread.h>
#include <unistd.h>  // usleep

static const int TARGET_COUNT = 100;

typedef struct {
    double_buffer_state_t state;
    int processed_count;
    int failed_mark_ready;
} test_shared_t;

void* isr_thread_fn(void* arg) {
    test_shared_t *shared = (test_shared_t *)arg;

    for (int i = 0; i < TARGET_COUNT; ++i) {
        while (!dbuf_mark_ready(&shared->state)) {
            usleep(50); // Wait for main to catch up
        }
        usleep(100);  // Simulate ISR delay
    }
    return NULL;
}

void* main_thread_fn(void* arg) {
    test_shared_t *shared = (test_shared_t *)arg;

    while (shared->processed_count < TARGET_COUNT) {
        int8_t ready = dbuf_get_ready(&shared->state);
        if (ready != NO_BUFFER_READY) {
            dbuf_mark_processed(&shared->state);
            shared->processed_count++;
        }
        usleep(150);  // Simulate main loop delay
    }
    return NULL;
}

void test_isr_and_main_toggle_safely(void) {
    test_shared_t shared = {0};
    dbuf_init(&shared.state);

    pthread_t isr_thread, main_thread;
    pthread_create(&isr_thread, NULL, isr_thread_fn, &shared);
    pthread_create(&main_thread, NULL, main_thread_fn, &shared);

    pthread_join(isr_thread, NULL);
    pthread_join(main_thread, NULL);

    TEST_ASSERT_EQUAL_INT(TARGET_COUNT, shared.processed_count);
    TEST_ASSERT_EQUAL_INT8(NO_BUFFER_READY, dbuf_get_ready(&shared.state));
}

void* racey_isr_thread(void* arg) {
    test_shared_t *shared = (test_shared_t *)arg;

    for (int i = 0; i < TARGET_COUNT; ++i) {
        if (!dbuf_mark_ready(&shared->state)) {
            shared->failed_mark_ready++;
        }
        usleep(20);
    }
    return NULL;
}

void* racey_main_thread(void* arg) {
    test_shared_t *shared = (test_shared_t *)arg;

    for (int i = 0; i < TARGET_COUNT; ++i) {
        int8_t ready = dbuf_get_ready(&shared->state);
        if (ready != NO_BUFFER_READY) {
            dbuf_mark_processed(&shared->state);
        }
        usleep(30);
    }
    return NULL;
}

void test_no_double_ready_race(void) {
    test_shared_t shared = {0};
    dbuf_init(&shared.state);

    pthread_t isr_thread, main_thread;
    pthread_create(&isr_thread, NULL, racey_isr_thread, &shared);
    pthread_create(&main_thread, NULL, racey_main_thread, &shared);

    pthread_join(isr_thread, NULL);
    pthread_join(main_thread, NULL);

    // At least one mark_ready attempt should have failed due to the buffer being in use
    TEST_ASSERT_GREATER_THAN(0, shared.failed_mark_ready);
}

#define TEST_BUF_SIZE 128
#define NUM_LINES 10

typedef struct {
    double_buffer_state_t state;
    uint8_t buf0[TEST_BUF_SIZE];
    uint8_t buf1[TEST_BUF_SIZE];
    uint32_t buf0_idx;
    uint32_t buf1_idx;

    char lines_received[NUM_LINES][TEST_BUF_SIZE];
    int lines_processed;

    const char *test_lines[NUM_LINES];
} full_test_shared_t;

void *test_isr_thread(void *arg) {
    full_test_shared_t *shared = (full_test_shared_t *)arg;

    for (int i = 0; i < NUM_LINES; ++i) {
        const char *line = shared->test_lines[i];
        size_t len = strlen(line);

        for (size_t j = 0; j < len; ++j) {
            char c = line[j];
            uint8_t buf_idx = dbuf_get_isr_index(&shared->state);
            uint8_t *buf = (buf_idx == 0) ? shared->buf0 : shared->buf1;
            uint32_t *p_idx = (buf_idx == 0) ? &shared->buf0_idx : &shared->buf1_idx;

            buf[*p_idx] = c;
            (*p_idx)++;

            if (c == '\n') {
                // Null-terminate line
                if (*p_idx < TEST_BUF_SIZE) {
                    buf[*p_idx] = '\0';
                } else {
                    buf[TEST_BUF_SIZE - 1] = '\0';
                }

                dbuf_mark_ready(&shared->state);

                // Reset pointer for next line
                if (buf_idx == 0) shared->buf0_idx = 0;
                else shared->buf1_idx = 0;
            }

            usleep(200);  // Simulate typing
        }
    }

    return NULL;
}

void *test_main_thread(void *arg) {
    full_test_shared_t *shared = (full_test_shared_t *)arg;

    while (shared->lines_processed < NUM_LINES) {
        int8_t ready = dbuf_get_ready(&shared->state);
        if (ready != NO_BUFFER_READY) {
            uint8_t *buf = (ready == 0) ? shared->buf0 : shared->buf1;

            // Copy the line for validation
            strncpy(shared->lines_received[shared->lines_processed], (char *)buf, TEST_BUF_SIZE);
            shared->lines_received[shared->lines_processed][TEST_BUF_SIZE - 1] = '\0';
            shared->lines_processed++;

            dbuf_mark_processed(&shared->state);
        }

        usleep(100);  // Simulate main processing delay
    }

    return NULL;
}

void test_full_buffer_flow_with_real_data(void) {
    full_test_shared_t shared = {0};
    dbuf_init(&shared.state);

    const char *test_lines[] = {
        "hello world\n",
        "how are you?\n",
        "testing 1 2 3\n",
        "foo bar\n",
        "double buffering!\n",
        "ceiling test\n",
        "quick brown fox\n",
        "lorem ipsum\n",
        "embedded c rules\n",
        "last line\n"
    };
    memcpy(shared.test_lines, test_lines, sizeof(test_lines));

    pthread_t isr_thread, main_thread;
    pthread_create(&isr_thread, NULL, test_isr_thread, &shared);
    pthread_create(&main_thread, NULL, test_main_thread, &shared);

    pthread_join(isr_thread, NULL);
    pthread_join(main_thread, NULL);

    TEST_ASSERT_EQUAL_INT(NUM_LINES, shared.lines_processed);

    for (int i = 0; i < NUM_LINES; ++i) {
        TEST_ASSERT_EQUAL_STRING_MESSAGE(shared.test_lines[i], shared.lines_received[i], "Line mismatch");
    }
}
