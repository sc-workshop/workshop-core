#pragma once

#include "core/memory/memory.h"
#include "core/preprocessor/api.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
{
	struct SUPERCELL_API Vector4D
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};

			struct
			{
				float s, t, u, v;
			};

			struct
			{
				float r, g, b, a;
			};

			float coords[4];
		};

		Vector4D(const float* arr)
		{
			Memory::copy<float>(arr, coords, sizeof(coords));
		}

		Vector4D(float a = 0.0f, float b = 0.0f, float c = 0.0f, float d = 0.0f)
		{
			x = a;
			y = b;
			z = c;
			w = d;
		}
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
