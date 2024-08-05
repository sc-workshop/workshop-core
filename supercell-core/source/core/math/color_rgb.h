#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
{
	template<typename T = uint8_t>
	struct ColorRGB
	{
		union
		{
			struct
			{
				T r, g, b;
			};

			T channels[3];
		};

		ColorRGB(const T* arr)
		{
			Memory::copy(arr, channels, sizeof(channels));
		};

		ColorRGB(T red = 0, T green = 0, T blue = 0)
		{
			r = red;
			g = green;
			b = blue;
		};
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif