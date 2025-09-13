#include "fnv.h"

namespace wk::hash
{
	uint32_t fnv1a32(const void* data, std::size_t length)
	{
		uint32_t hash = 2166136261U;

		for (std::size_t i = 0; length > i; ++i) {
			hash = 16777619U * (hash ^ *((const uint8_t*)data + i));
		}

		return hash;
	}

	uint64_t fnv1a64(const void* data, std::size_t length)
	{
		uint64_t hash = 14695981039346656037ULL;

		for (std::size_t i = 0; length > i; ++i) {
			hash = 1099511628211ULL * (hash ^ *((const uint8_t*)data + i));
		}

		return hash;
	}
}