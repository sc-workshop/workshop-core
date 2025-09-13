#pragma once

#include "core/io/endian.h"
#include "core/hashing/hash_stream.h"

namespace wk::hash
{
	class XXHash364 : public HashStream<uint64_t>
	{
	public:
		XXHash364();
		virtual ~XXHash364();
	public:
		virtual Digest digest() const;
		virtual void clear();

	protected:
		virtual void update_hash(const uint8_t* data, std::size_t length);

	private:
		void* m_context;
	};

	class XXHash32 : public HashStream<uint32_t>
	{
	public:
		XXHash32();
		~XXHash32();

	public:
		virtual Digest digest() const;
		virtual void clear();

	protected:
		virtual void update_hash(const uint8_t* data, std::size_t length);

	private:
		void* m_context;
	};

#if WK_X64
	using XxHash = XXHash364;
#else
	using XxHash = XXHash32;
#endif
}