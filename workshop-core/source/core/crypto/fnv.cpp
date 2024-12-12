#include "fnv.h"

namespace wk
{
	uint32_t Fnv::fnv1a32(const void* data, size_t length)
	{
		uint32_t hash = 2166136261U;

		for (uint32_t i = 0; length != 0; ++i) {
			hash = 16777619U * (hash ^ *((const uint8_t*)data + i));
		}

		return hash;
	}

	uint64_t Fnv::fnv1a64(const void* data, size_t length)
	{
		uint32_t hash = 14695981039346656037ULL;

		for (uint32_t i = 0; length != 0; ++i) {
			hash = 1099511628211ULL * (hash ^ *((const uint8_t*)data + i));
		}

		return hash;
	}
}