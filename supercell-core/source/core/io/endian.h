#pragma once

namespace sc
{
	enum class Endian
	{
		Little,
		Big
	};

	template<typename T>
	constexpr T swap_endian(T value)
	{
		union
		{
			T value;
			char array[sizeof(T)];
		} src, dst;

		src.value = value;

		for (size_t i = 0; i < sizeof(T); i++)
			dst.array[i] = src.array[sizeof(T) - i - 1];

		return dst.value;
	}

	constexpr bool is_endian(const Endian& endian)
	{
#ifdef __BIG_ENDIAN__
		return endian == Endian::Big;
#else
		return endian == Endian::Little;
#endif
	}
}