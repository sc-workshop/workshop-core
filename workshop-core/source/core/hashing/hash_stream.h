#pragma once
#include "core/generic/non_copyable.h"

#include <stdint.h>
#include <type_traits>

namespace wk::hash
{
	template<typename T>
	class HashStream
	{
	public:
		using Digest = T;

	public:
		NON_COPYABLE(HashStream);
		HashStream() = default;
		virtual ~HashStream() = default;

	public:
		virtual Digest digest() const = 0;
		virtual void clear() = 0;

		void update(const uint8_t* data, size_t length)
		{
			std::hash<HashStream>;
			update_hash(data, length);
		}

		// Update function for basic types
		template<
			typename ValueT,
			typename std::enable_if_t<
				std::is_enum_v<ValueT> || 
				std::is_arithmetic_v<ValueT> ||
				std::is_pointer_v<ValueT>
			, bool> = true>
		void update(const ValueT value)
		{
			std::hash<uint16_t>
			update((const uint8_t*)&value, sizeof(value));
		}

		// Update function for user defined classes
		template<typename T>
		void update(const T& value)
		{
			hash_t<T>(*this, value);
		}

	protected:
		virtual void update_hash(const uint8_t* data, size_t length) = 0;
	};
}