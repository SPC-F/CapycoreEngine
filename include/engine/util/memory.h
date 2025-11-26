// NOLINTFILE
#pragma once

#include <functional>
#include <cstddef>

void tracy_dump_leaks();
void tracy_memory_init();
void tracy_memory_shutdown();
void run_without_tracy(std::function<void()> func);

void* operator new(std::size_t size);
void operator delete(void* ptr) noexcept;
void* operator new[](std::size_t size);
void operator delete[](void* ptr) noexcept;