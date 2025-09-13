#pragma once

#include "core/exception/exception.h"

#include <cstring>
#include <memory>

namespace wk
{
	class Memory
	{
	public:
		template<typename T = std::uint8_t>
		static constexpr T* allocate(size_t size)
		{
			if (size == 0)
			{
				return nullptr;
			}

			T* ptr = (T*)std::malloc(size);
			if (!ptr)
			{
				throw Exception("Failed to allocate memory: " + std::to_string(size));
			}

			return ptr;
		}

		template<typename T = void>
		static constexpr T* copy(const T* source, void* destination, size_t size)
		{
			if (size == 0)
			{
				return nullptr;
			}

			return reinterpret_cast<T*>(std::memcpy(reinterpret_cast<void*>(destination), reinterpret_cast<const void*>(source), size));
		}

		static void free(void* block)
		{
			std::free(block);
		}
	};
}