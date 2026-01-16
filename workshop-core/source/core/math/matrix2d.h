#pragma once

#include "core/memory/memory.h"
#include "core/preprocessor/api.h"
#include "core/hashing/hashing.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

namespace wk
{
	struct WORKSHOP_API Matrix2D
	{
	public:
		static const Matrix2D& Identity()
		{
			static Matrix2D identity;
			return identity;
		}

	public:
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

		bool operator == (const Matrix2D& other) const
		{
			return a == other.a && b == other.b &&
				c == other.c && d == other.d &&
				tx == other.tx && ty == other.ty;
		};

		float applyX(float x, float y) const {
			return x * a + y * c + tx;
		}

		float applyY(float x, float y) const {
			return y * d + x * b + ty;
		}

		template<typename T = Matrix2D>
		T operator*(const T& matrix) const {
			T result{};
			float scaleX = (a * matrix.a) + (b * matrix.c);
			float shearX = (a * matrix.b) + (b * matrix.d);
			float scaleY = (d * matrix.d) + (c * matrix.b);
			float shearY = (d * matrix.c) + (c * matrix.a);
			float x = matrix.applyX(tx, ty);
			float y = matrix.applyY(tx, ty);

			result.a = scaleX;
			result.b = shearX;
			result.c = shearY;
			result.d = scaleY;
			result.tx = x;
			result.ty = y;
			
			return result;
		}
	};
}

namespace wk::hash
{
	template<>
	struct Hash_t<wk::Matrix2D>
	{
		template<typename T>
		static void update(wk::hash::HashStream<T>& stream, const wk::Matrix2D& matrix)
		{
			stream.update((const uint8_t*)matrix.elements, sizeof(matrix.elements));
		}
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
