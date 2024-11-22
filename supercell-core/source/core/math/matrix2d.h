#pragma once

#include "core/memory/memory.h"
#include "core/preprocessor/api.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
{
	struct SUPERCELL_API Matrix2D
	{
		union
		{
			struct
			{
				float a, b, c, d, tx, ty;
			};

			float elements[6];
		};

		Matrix2D(const float* arr)
		{
			Memory::copy(arr, elements, sizeof(elements));
		};

		Matrix2D(
			float m00 = 1.0f,
			float m01 = 0.0f,
			float m10 = 0.0f,
			float m11 = 1.0f,
			float m02 = 0.0f,
			float m12 = 0.0f
		)
		{
			a = m00;
			b = m01;

			c = m10;
			d = m11;

			tx = m02;
			ty = m12;
		};
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
