#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
{
	template<typename T>
	struct Rect_t
	{
		union
		{
			struct
			{
				T x, y;
				T width, height;
			};

			struct
			{
				T left, top;
				T right, bottom;
			};

			T points[4];
		};

		Rect(const T* arr)
		{
			Memory::copy(arr, points, sizeof(points));
		};

		Rect(T a = 0.0f, T b = 0.0f, T c = 0.0f, T d = 0.0f)
		{
			x = a;
			y = b;
			width = c;
			height = d;
		};
	};

	using Rect = Rect_t<int>;
	using RectF = Rect_t<float>;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
