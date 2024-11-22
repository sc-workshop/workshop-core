#pragma once

#include "core/memory/memory.h"
#include "core/preprocessor/api.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace sc
{
	struct SUPERCELL_API Vector3D
	{
		union
		{
			struct
			{
				float x, y, z;
			};

			struct
			{
				float u, v, w;
			};

			struct
			{
				float s, t, w;
			};

			struct
			{
				float r, g, b;
			};

			float coords[3];
		};

		Vector3D(const float* arr)
		{
			Memory::copy<float>(arr, coords, sizeof(coords));
		}

		Vector3D(float a = 0.0f, float b = 0.0f, float c = 0.0f)
		{
			x = a;
			y = b;
			z = c;
		}
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
