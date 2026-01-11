// NOLINTFILE
#pragma once

#include <cstddef>
#include <functional>

/**
 * @brief Memory management utilities and custom new/delete operators.
 * This header provides functions for initializing and shutting down
 * memory tracking, as well as custom global new and delete operators
 * that integrate with the Tracy profiler for memory leak detection.
 *
 * @warning These overrides are meant to be used with the Tracy profiler.
 * If Tracy is not being used, the run_without_tracy function allows
 * executing code without Tracy's memory tracking.
 *
 * @warning This should not be used in release builds where performance is
 * critical, as the custom new/delete operators may introduce overhead.
 */

void tracy_dump_leaks();
void tracy_memory_init();
void tracy_memory_shutdown();
void run_without_tracy(std::function<void()> func);

void* operator new(std::size_t size);
void operator delete(void* ptr) noexcept;
void* operator new[](std::size_t size);
void operator delete[](void* ptr) noexcept;