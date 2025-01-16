#pragma once

#include "core/memory/memory.h"
#include "core/hashing/hash.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace wk
{
	template<typename T = uint8_t>
	struct ColorRGB_t
	{
		union
		{
			struct
			{
				T r, g, b;
			};

			T channels[3];
		};

		ColorRGB_t(const T* arr)
		{
			Memory::copy(arr, channels, sizeof(channels));
		};

		ColorRGB_t(T red = 0, T green = 0, T blue = 0)
		{
			r = red;
			g = green;
			b = blue;
		};

		bool operator==(const ColorRGB_t<T>& other) const
		{
			return std::memcmp(&channels, &other.channels, sizeof(channels)) == 0;
		}

		bool operator!=(const ColorRGB_t<T>& other) const
		{
			return !(*this == other);
		}
	};

	using ColorRGB = ColorRGB_t<uint8_t>;

	namespace hash
	{
		template<typename V>
		struct Hash_t<wk::ColorRGB_t<V>>
		{
			template<typename T>
			static void update(wk::hash::HashStream<T>& stream, const wk::ColorRGB_t<V>& color)
			{
				stream.update((uint8_t*)&color.channels, sizeof(color.channels));
			}
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif