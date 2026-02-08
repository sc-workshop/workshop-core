#include "md5.h"
#include <cstring>

namespace wk::MD5
{
	#include "core/3rdparty/md5/md5.h"
}


namespace wk::hash
{
	MD5::MD5() 
	{
        clear();
	}

	MD5::Digest MD5::digest() const
	{
		MD5::Digest result{};
		m_context->final(result.data());
		return result;
	}

	void MD5::update_hash(const uint8_t* data, size_t length)
	{
		m_context->update(data, length);
	}

	void MD5::clear()
	{
        m_context = wk::CreateRef<wk::MD5::md5>();
	}
}