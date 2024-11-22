#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace wk
{
	template<typename T>
	struct Line_t
	{
		using ValueT = Point_t<T>;

		union
		{
			struct
			{
				ValueT start, end;
			};

			ValueT points[2];
		};

		Line_t(const T* arr)
		{
			Memory::copy(arr, points, sizeof(points));
		};

		Line_t(ValueT _start = { 0.f, 0.f }, ValueT _end = { 0.f, 0.f })
		{
			start = _start;
			end = _end;
		};
	};

	using Line = Line_t<int>;
	using LineF = Line_t<float>;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
