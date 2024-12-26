#pragma once
#include "core/generic/non_copyable.h"

#include <stdint.h>
#include <type_traits>

#include <hash_map>

namespace wk
{
	template<typename T>
	class HashStream
	{
	public:
		using Digest = T;

	public:
		NON_COPYABLE(HashStream);
		virtual ~HashStream() = default;

	public:
		virtual Digest digest() const = 0;

		void update(const uint8_t* data, size_t length)
		{
			update_hash(data, length);
		}

		template<typename ValueT, typename = !std::is_volatile<T>
											&& (std::is_enum<T> || std::is_integral<T> || std::is_pointer<T>)>
		void update(const ValueT value)
		{
			update((const uint8_t*)&value, sizeof(value));
		}

	protected:
		virtual void update_hash(const uint8_t* data, size_t length) = 0;
	};
}