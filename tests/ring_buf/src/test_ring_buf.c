/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	int return_val;
	int test_val = 99;
	
	rb_push(test_val);
	zassert_equal(rb_count(), 1, "Count after first push should be 1");

	rb_peek(&return_val);
	zassert_equal(return_val, test_val, "Value peeked after push should be equal to test value");

	rb_init(4);
	zassert_true(rb_is_empty(), "Reinitialized buffer must be empty");
	zassert_equal(rb_count(), 0, "Reinitialized count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	
	int test_val = 42;
	int return_val;
	
	rb_push(test_val);
	zassert_equal(rb_count(), 1, "FIFO data count should be 1");
	
	rb_pop(&return_val);
	zassert_equal(return_val, test_val, "Popped value should be equal to pushed value");
	
	zassert_equal(rb_count(), 0, "Buffer should be empty after last pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	
	rb_push(1);
	rb_push(2);
	rb_push(3);
	
	int val;
	rb_pop(&val);
	zassert_equal(val, 1, "First value popped should be 1");
	rb_pop(&val);
	zassert_equal(val, 2, "Second value popped should be 2");
	rb_pop(&val);
	zassert_equal(val, 3, "Third value popped should be 3");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	
	int test_vals[5] = {1, 2, 3, 4, 5};
	
	for (uint8_t i=0; i<4; i++) {
		rb_push(test_vals[i]);
	}
	zassert_equal(rb_count(), 4, "Count after 4 pushes should be 4");
	
	zassert_equal(rb_push(test_vals[4]), -ENOSPC, "Full buffer should return -ENOSPC");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	
	int val;
	
	rb_push(7);
	rb_peek(&val);
	zassert_equal(val, 7, "Peek should return 7");
	
	rb_peek(&val);
	zassert_equal(val, 7, "Second peek should also return 7");
	zassert_equal(rb_count(), 1, "Count should still be 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	
	zassert_equal(rb_pop(NULL), -EINVAL, "Pop to a null pointer should return -EINVAL");	
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	
	int test_vals[4] = {1, 2, 3, 4};
	
	for (uint8_t i=0; i<4; i++) {
		rb_push(test_vals[i]);
	}
	
	zassert_true(rb_is_full(), "Buffer should be full after fill");
	zassert_equal(rb_count(), 4, "Buffer should hold 4 values now");
}
