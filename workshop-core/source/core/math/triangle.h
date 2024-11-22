#pragma once

#include "core/memory/memory.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace wk
{
	template<typename T>
	struct Triangle_t
	{
		using ValueT = Point_t<T>;

		union
		{
			struct
			{
				ValueT p1, p2, p3;
			};

			ValueT points[3];
		};

		Triangle_t(const T* arr)
		{
			Memory::copy(arr, points, sizeof(points));
		};

		Triangle_t(ValueT _p1 = { 0.f, 0.f }, ValueT _p2 = { 0.f, 0.f }, ValueT _p3 = { 0.f, 0.f } )
		{
			p1 = _p1;
			p2 = _p2;
			p3 = _p3;
		};
	};

	using Triangle = Triangle_t<int>;
	using TriangleF = Triangle_t<float>;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
