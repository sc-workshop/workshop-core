#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
{
	template<typename T = uint8_t>
	struct ColorRGBA
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
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
