#pragma once

class MemoryPool
{
public:
	void* allocate(size_t bytes);
	void free(void* ptr);

	MemoryPool();
	~MemoryPool();
};
