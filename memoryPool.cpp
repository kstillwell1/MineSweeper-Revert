#include "memoryPool.h"
#include "tile.h"

MemoryPool::MemoryPool()
{

}

MemoryPool::~MemoryPool()
{

}

Tile* MemoryPool::allocate(size_t count)
{
    return new Tile[count];
}

void MemoryPool::free(Tile* ptr)
{
    delete[] ptr;
}
