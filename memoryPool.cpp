#include "memoryPool.h"

MemoryPool::MemoryPool()
{

}

MemoryPool::~MemoryPool()
{

}

void* MemoryPool::allocate(size_t bytes)
{
    return ::operator new(bytes);
}

void MemoryPool::free(void* ptr)
{
    ::operator delete(ptr);
}
