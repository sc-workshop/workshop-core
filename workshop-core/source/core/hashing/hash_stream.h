#pragma once
#include "core/generic/non_copyable.h"

#include <stdint.h>
#include <type_traits>
#include "hash.h"

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
			if (!length) return;
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
			update((const uint8_t*)&value, sizeof(value));
		}

		// Update function for user defined classes
		template<
			typename ValueT,
			typename std::enable_if_t<
				std::is_class_v<ValueT>
			, bool> = true>
		void update(const ValueT & value)
		{
			Hash_t<ValueT>::template update<T>(*this, value);
		}

	public:
		template<typename H>
		static H combine(const H& seed, const H& value)
		{
			return value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

	protected:
		virtual void update_hash(const uint8_t* data, size_t length) = 0;
	};
}