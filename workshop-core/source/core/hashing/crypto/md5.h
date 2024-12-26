#pragma once

#include <array>
#include "core/hashing/hash_stream.h"
#include "core/memory/ref.h"

namespace wk::MD5
{
	class md5;
}

namespace wk::hash
{
	using MD5Digest = std::array<uint8_t, 16>;
	class MD5 : public HashStream<MD5Digest>
	{
	public:
		MD5();
		virtual ~MD5();

	public:
		virtual Digest digest() const;
		virtual void clear();

	protected:
		virtual void update_hash(const uint8_t* data, size_t length);

	private:
		wk::MD5::md5* m_context;
	};
}
