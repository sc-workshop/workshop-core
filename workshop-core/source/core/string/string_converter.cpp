#ifdef WK_LOCALE
#include "string_converter.h"

#include "simdutf.h"

namespace wk
{
	std::string StringConverter::ToUTF8(const std::u16string& str)
	{
        std::string result;
        size_t needed = simdutf::utf8_length_from_utf16(str.data(), str.size());
        result.resize(needed);

        size_t length = simdutf::convert_utf16_to_utf8(str.data(), str.size(), result.data());
        result.resize(length);
        return result;
	}

	std::u16string StringConverter::ToUTF16(const std::string& str)
	{
        std::u16string result;
        size_t needed = simdutf::utf16_length_from_utf8(str.data(), str.size());
        result.resize(needed);

        size_t length = simdutf::convert_utf8_to_utf16(str.data(), str.size(), result.data());
        result.resize(length);
        return result;
	}
}
#endif
