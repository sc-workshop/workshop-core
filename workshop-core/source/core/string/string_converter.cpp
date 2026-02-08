#ifdef WK_LOCALE
#include "string_converter.h"

#include <simdturf.h>

namespace wk
{
	std::string StringConverter::ToUTF8(const std::u16string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		return convert.to_bytes(str);
	}

	std::u16string StringConverter::ToUTF16(const std::string& str)
	{
		static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		return convert.from_bytes(str);
	}
}
#endif
