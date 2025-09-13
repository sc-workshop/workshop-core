#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace wk
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

		template <typename O>
		bool operator<(const Point_t<O>& other) const
		{
			return std::tie(x, y) < std::tie(other.x, other.y);
		}

		bool operator==(const Point_t<T>& other) const
		{
			return x == other.x && y == other.y;
		}
	};

	using Point = Point_t<int>;
	using PointF = Point_t<float>;
	using ColorLA = Point_t<uint8_t>;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
