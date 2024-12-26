#pragma once

#include <stdint.h>
#include "core/hashing/hash_stream.h"

namespace wk::hash
{
	template<typename T, T (*hash_f)(const void*, size_t)>
	class Fnv : public HashStream<T>
	{
	public:
		virtual Digest digest() const
		{
			return m_hash;
		}
		virtual void clear()
		{
			m_hash = 0;
		}

	protected:
		virtual void update_hash(const uint8_t* data, size_t length)
		{
			T hash = hash_f(data, length);
			m_hash = HashStream::combine(m_hash, hash);
		}

	private:
		T m_hash = 0;
	};

	uint32_t fnv1a32(const void* data, size_t length);
	uint64_t fnv1a64(const void* data, size_t length);

	using Fnv1a32 = Fnv<uint32_t, fnv1a32>;
	using Fnv1a64 = Fnv<uint64_t, fnv1a64>;
}