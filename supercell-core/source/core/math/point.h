#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
{
	template<typename T>
	struct Point_t
	{
		union
		{
			struct
			{
				T x, y;
			};

			struct
			{
				T u, v;
			};

			struct
			{
				T s, t;
			};

			struct
			{
				T r, g;
			};

			struct
			{
				T l, a;
			};

			T coords[2];
		};

		Point_t(const T* arr)
		{
			Memory::copy(arr, coords, sizeof(coords));
		}

		Point_t(T a = 0.0f, T b = 0.0f)
		{
			x = a;
			y = b;
		};
	};

	using Point = Point_t<int>;
	using PointF = Point_t<float>;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
