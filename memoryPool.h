#pragma once
#include "tile.h"

class MemoryPool
{
public:
	Tile* allocate(size_t count);
	void free(Tile* ptr);

	MemoryPool();
	~MemoryPool();
};
