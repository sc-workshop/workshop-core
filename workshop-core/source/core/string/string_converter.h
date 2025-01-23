#pragma once

#include "core/generic/static.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace wk
{
	class StringConverter
	{
	private:
		WK_STATIC_CLASS(StringConverter);

	public:
		static std::string ToUTF8(const std::u16string& str);
		static std::u16string ToUTF16(const std::string& str);

	public:
		template<typename T, typename S>
		static void ToHex(S& ss, const T& value)
		{
			ss << std::setw(sizeof(T) * 2) << (int)value;
		}

		template<typename... T>
		static std::string ToHex(T&&... values)
		{
			std::stringstream ss{};
			ss << std::hex << std::setfill('0');

			([&]
				{
					StringConverter::ToHex(ss, values);
				} (), ...);

			return ss.str();
		}

		static void StripLeadingHexZeros(std::string& str)
		{
			str.erase(0, str.find_first_not_of('0'));
		}
	};
}