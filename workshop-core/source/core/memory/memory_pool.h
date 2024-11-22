#pragma once

#include "core/3rdparty/Allocator/STL/MemoryPool.h"

namespace wk
{
	using MemoryPool = STL::MemoryPool;

	template <typename T, typename S = std::size_t>
	using MemoryPoolAllocator = STL::MemoryPoolAllocator<T, S>;
}
