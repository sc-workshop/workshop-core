#include "md5.h"

namespace wk::MD5
{
	#include "core/3rdparty/md5/md5.h"
}


namespace wk::hash
{
	MD5::MD5()
	{
		m_context = new wk::MD5::md5();
	}

	MD5::~MD5()
	{
		delete m_context;
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
		delete m_context;
		m_context = new wk::MD5::md5();
	}
}