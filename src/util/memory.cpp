#include <unordered_map>
#include <mutex>
#include <iostream>
#include <cstdlib>

#include <engine/util/memory.h>

#include <tracy/Tracy.hpp>

namespace {
    std::recursive_mutex g_mutex;
    std::unordered_map<void*, size_t> g_allocations;
    bool g_trackingEnabled = true;
} 

void tracy_dump_leaks()
{
    std::lock_guard<std::recursive_mutex> lock(g_mutex);

    if (g_allocations.empty()) {
        std::cout << "[MEMORY] No memory leaks detected." << std::endl;
        return;
    }

    std::cout << "[MEMORY] Detected memory leaks: " << g_allocations.size() << std::endl;
    size_t total = 0;

    for (auto& [ptr, size] : g_allocations) {
        std::cout << "[MEMORY] Leak at " << ptr << ", size: " << size << " bytes" << std::endl;
        total += size;
    }

    std::cout << "[MEMORY] Total leaked memory: " << total << " bytes 🤡" << std::endl;
}

void tracy_memory_init()
{
    std::lock_guard<std::recursive_mutex> lock(g_mutex);
    g_trackingEnabled = true;
}

void tracy_memory_shutdown()
{
    std::lock_guard<std::recursive_mutex> lock(g_mutex);
    g_trackingEnabled = false;
    tracy_dump_leaks();
}

// =========================================================
// Override new / delete operators
// =========================================================
void* operator new(std::size_t sz)
{
    if (sz == 0)
    {
        ++sz;
    }
 
    if (void *ptr = std::malloc(sz))
    {
        if (g_trackingEnabled)
        {
            // We temporarily disable tracking to prevent recursion
            g_trackingEnabled = false;
            {
                std::lock_guard<std::recursive_mutex> lock(g_mutex);
                g_allocations[ptr] = sz;
                TracyAlloc(ptr, sz);
            }
            g_trackingEnabled = true;
        }

        return ptr;
    }
 
    throw std::bad_alloc{};
}

void* operator new[](std::size_t sz)
{
    if (sz == 0)
    {
        ++sz; // to avoid std::malloc(0) which may return nullptr on success
    }
 
    if (void *ptr = std::malloc(sz))
    {
        if (g_trackingEnabled)
        {
            // We temporarily disable tracking to prevent recursion
            g_trackingEnabled = false;
            {
                std::lock_guard<std::recursive_mutex> lock(g_mutex);
                g_allocations[ptr] = sz;
                TracyAlloc(ptr, sz);
            }
            g_trackingEnabled = true;
        }

        return ptr;
    }
 
    throw std::bad_alloc{}; // required by [new.delete.single]/3
}
 
void operator delete(void* ptr) noexcept
{
    if (g_trackingEnabled && ptr)
    {
        g_trackingEnabled = false;
        {
            std::lock_guard<std::recursive_mutex> lock(g_mutex);
            auto it = g_allocations.find(ptr);
            if (it != g_allocations.end())
            {
                TracyFree(ptr);
                g_allocations.erase(it);
            }
        }
        g_trackingEnabled = true;
    }

    std::free(ptr);
}
 
void operator delete(void* ptr, std::size_t size) noexcept
{
    if (g_trackingEnabled && ptr)
    {
        g_trackingEnabled = false;
        {
            std::lock_guard<std::recursive_mutex> lock(g_mutex);
            auto it = g_allocations.find(ptr);
            if (it != g_allocations.end())
            {
                TracyFree(ptr);
                g_allocations.erase(it);
            }
        }
        g_trackingEnabled = true;
    }

    std::free(ptr);
}
 
void operator delete[](void* ptr) noexcept
{
    if (g_trackingEnabled && ptr)
    {
        g_trackingEnabled = false;
        {
            std::lock_guard<std::recursive_mutex> lock(g_mutex);
            auto it = g_allocations.find(ptr);
            if (it != g_allocations.end())
            {
                TracyFree(ptr);
                g_allocations.erase(it);
            }
        }
        g_trackingEnabled = true;
    }

    std::free(ptr);
}
 
void operator delete[](void* ptr, std::size_t size) noexcept
{
    if (g_trackingEnabled && ptr)
    {
        g_trackingEnabled = false;
        {
            std::lock_guard<std::recursive_mutex> lock(g_mutex);
            auto it = g_allocations.find(ptr);
            if (it != g_allocations.end())
            {
                TracyFree(ptr);
                g_allocations.erase(it);
            }
        }
        g_trackingEnabled = true;
    }

    std::free(ptr);
}
