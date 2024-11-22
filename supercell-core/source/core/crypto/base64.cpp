#include "core/crypto/base64.h"

#include "core/3rdparty/cpp-base64/base64.h"

namespace sc
{
	std::string Base64::encode(const std::string& data, bool url_safe)
	{
		return base64_encode(data, url_safe);
	}

	std::string Base64::decode(const std::string& data)
	{
		return base64_decode(data);
	}
}
