#pragma once

#include "core/memory/memory.h"
#include "core/hashing/hash.h"

#include <stdint.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace wk
{
	template<typename T>
	struct ColorRGBA_t
	{
		union
		{
			struct
			{
				T r, g, b, a;
			};

			T channels[4];
		};

		ColorRGBA_t(const T* arr)
		{
			Memory::copy(arr, channels, sizeof(channels));
		};

		ColorRGBA_t(T red = 0, T green = 0, T blue = 0, T alpha = 0)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		};

		uint32_t as_value() const
		{
			return *(uint32_t*)this;
		}
		
		void set_value(uint32_t value) const
		{
			*(uint32_t*)this = value;
		}

		bool operator==(const ColorRGBA_t<T>& other) const
		{
			return std::memcmp(&channels, &other.channels, sizeof(channels)) == 0;
		}

		bool operator!=(const ColorRGBA_t<T>& other) const
		{
			return !(*this == other);
		}
	};

	using ColorRGBA = ColorRGBA_t<uint8_t>;

	namespace hash
	{
		template<typename V>
		struct Hash_t<wk::ColorRGBA_t<V>>
		{
			template<typename T>
			static void update(wk::hash::HashStream<T>& stream, const wk::ColorRGBA_t<V>& color)
			{
				stream.update((uint8_t*)&color.channels, sizeof(color.channels));
			}
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
