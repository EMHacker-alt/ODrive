/**
 * Tests the monotonic system clock
 */

#include <stm32_system.h>

void error() {
    for (;;); // todo: report error
}

void success() {
    for (;;); // todo: report success
}

int main(void) {
    if (!system_init()) {
        error();
    }

    if (!init_monotonic_clock(&tim14, 255)) {
        error();
    }

    // Test if the there are any spurious forward or backward steps in the
    // monotonic clock
    delay_us(1000);
    for (size_t i = 0; i < 100000; ++i) {
        uint64_t start = get_ticks_us();
        uint64_t end = get_ticks_us();
        if ((end - start) > 10) {
            error();
        }
    }

    // Test if delay works
    uint64_t start = get_ticks_us();
    delay_us(1000);
    uint64_t end = get_ticks_us();
    if ((end - start) < 1000) {
        error();
    }

    success();
    for (;;);
}
