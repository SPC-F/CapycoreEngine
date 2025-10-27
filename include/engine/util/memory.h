#pragma once

#include <cstddef>

void tracy_dump_leaks();

void tracy_memory_init();
void tracy_memory_shutdown();

void* operator new(std::size_t size);
void operator delete(void* ptr) noexcept;
void* operator new[](std::size_t size);
void operator delete[](void* ptr) noexcept;