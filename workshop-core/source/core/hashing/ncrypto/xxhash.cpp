#include "xxhash.h"

#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION
#define XXH_INLINE_ALL

#ifdef __BIG_ENDIAN__
#define XXH_CPU_LITTLE_ENDIAN 0
#else
#define XXH_CPU_LITTLE_ENDIAN 1
#endif

#if WK_X86_64 && !WK_PREFERRED_LATEST_ISA
	#include "core/3rdparty/xxhash/xxh_x86dispatch.c"
#else
	#if WK_PREFERRED_OLDEST_ISA
		#define XXH_VECTOR XXH_SCALAR
	#elif WK_X86_64
		#define XXH_VECTOR XXH_AVX2
	#endif
#endif

#include "core/3rdparty/xxhash/xxhash.h"

namespace wk::hash
{
	XXHash364::XXHash364()
	{
		m_context = XXH3_createState();
		clear();
	}

	XXHash364::~XXHash364()
	{
		XXH3_freeState((XXH3_state_t*)m_context);
	}

	XXHash364::Digest XXHash364::digest() const
	{
		return XXH3_64bits_digest((XXH3_state_t*)m_context);
	}

	void XXHash364::clear()
	{
		XXH3_64bits_reset((XXH3_state_t*)m_context);
	}

	void XXHash364::update_hash(const uint8_t* data, size_t length)
	{
		XXH3_64bits_update((XXH3_state_t*)m_context, data, length);
	}

	XXHash32::XXHash32()
	{
		m_context = XXH32_createState();
		clear();
	}

	XXHash32::~XXHash32()
	{
		XXH32_freeState((XXH32_state_t*)m_context);
	}

	XXHash32::Digest XXHash32::digest() const
	{
		return XXH32_digest((XXH32_state_t*)m_context);
	}

	void XXHash32::clear()
	{
		XXH32_reset((XXH32_state_t*)m_context, 0);
	}

	void XXHash32::update_hash(const uint8_t* data, size_t length)
	{
		XXH32_update((XXH32_state_t*)m_context, data, length);
	}
}