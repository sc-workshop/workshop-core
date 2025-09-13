#pragma once

#include "core/memory/memory.h"
#include "core/hashing/hashing.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

#include <limits>

namespace wk
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

		Rect_t(const T* arr)
		{
			Memory::copy(arr, points, sizeof(points));
		};

		Rect_t(T a = 0.0f, T b = 0.0f, T c = 0.0f, T d = 0.0f)
		{
			x = a;
			y = b;
			width = c;
			height = d;
		};

		static const Rect_t<T>& Identity()
		{
			static Rect_t<T> identity(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
			return identity;
		}
	};

	using Rect = Rect_t<int>;
	using RectF = Rect_t<float>;
}

namespace wk::hash
{
	template<typename ValueT>
	struct Hash_t<Rect_t<ValueT>>
	{
		template<typename T>
		static void update(wk::hash::HashStream<T>& stream, const Rect_t<ValueT>& rect)
		{
			stream.update((const uint8_t*)&rect.points, sizeof(rect.points));
		}
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
