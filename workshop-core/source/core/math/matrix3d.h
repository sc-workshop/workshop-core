#pragma once

#include "core/memory/memory.h"
#include "core/preprocessor/api.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace wk
{
	struct WORKSHOP_API Matrix3D
	{
		union
		{
			struct
			{
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};

			float elements[16];
		};

		Matrix3D(const float* arr)
		{
			Memory::copy(arr, elements, sizeof(elements));
		};

		Matrix3D(
			float m00 = 1.0f,
			float m01 = 0.0f,
			float m02 = 0.0f,
			float m03 = 0.0f,

			float m10 = 0.0f,
			float m11 = 1.0f,
			float m12 = 0.0f,
			float m13 = 0.0f,

			float m20 = 0.0f,
			float m21 = 0.0f,
			float m22 = 1.0f,
			float m23 = 0.0f,

			float m30 = 0.0f,
			float m31 = 0.0f,
			float m32 = 0.0f,
			float m33 = 1.0f
		)
		{
			this->m00 = m00;
			this->m01 = m01;
			this->m02 = m02;
			this->m03 = m03;

			this->m10 = m10;
			this->m11 = m11;
			this->m12 = m12;
			this->m13 = m13;

			this->m20 = m20;
			this->m21 = m21;
			this->m22 = m22;
			this->m23 = m23;

			this->m30 = m30;
			this->m31 = m31;
			this->m32 = m32;
			this->m33 = m33;
		};
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
