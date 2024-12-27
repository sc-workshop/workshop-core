#include "xxhash.h"

namespace xxHash
{
#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION

#ifdef __BIG_ENDIAN__
#define XXH_CPU_LITTLE_ENDIAN 0
#else
#define XXH_CPU_LITTLE_ENDIAN 1
#endif
#include "core/3rdparty/xxhash/xxhash.h"
}

namespace wk::hash
{
	XXHash364::XXHash364()
	{
		m_context = xxHash::XXH3_createState();
		clear();
	}

	XXHash364::~XXHash364()
	{
		xxHash::XXH3_freeState(m_context);
	}

	XXHash364::Digest XXHash364::digest() const
	{
		return xxHash::XXH3_64bits_digest(m_context);
	}

	void XXHash364::clear()
	{
		xxHash::XXH3_64bits_reset(m_context);
	}

	void XXHash364::update_hash(const uint8_t* data, size_t length)
	{
		xxHash::XXH3_64bits_update(m_context, data, length);
	}

	XXHash32::XXHash32()
	{
		m_context = xxHash::XXH32_createState();
		clear();
	}

	XXHash32::~XXHash32()
	{
		xxHash::XXH32_freeState(m_context);
	}

	XXHash32::Digest XXHash32::digest() const
	{
		return xxHash::XXH32_digest(m_context);
	}

	void XXHash32::clear()
	{
		xxHash::XXH32_reset(m_context, 0);
	}

	void XXHash32::update_hash(const uint8_t* data, size_t length)
	{
		xxHash::XXH32_update(m_context, data, length);
	}
}