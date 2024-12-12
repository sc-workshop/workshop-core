#pragma once

#include <stdint.h>

namespace wk
{
	class Fnv
	{
	private:
		Fnv() = delete;

	public:
		static uint32_t fnv1a32(const void* data, size_t length);
		static uint64_t fnv1a64(const void* data, size_t length);
	};
}