#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
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

		ColorRGBA(const T* arr)
		{
			Memory::copy(arr, channels, sizeof(channels));
		};

		ColorRGBA(T red = 0, T green = 0, T blue = 0, T alpha = 0)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		};
	};

	using ColorRGBA = ColorRGBA_t<uint8_t>;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
