#pragma once

#include <string>

#include "core/preprocessor/api.h"

namespace wk
{
	class WORKSHOP_API Base64
	{
	private:
		Base64() = delete;

	public:
		static std::string encode(const std::string& data, bool url_safe = false);
		static std::string decode(const std::string& data);
	};
}
