#pragma once

#include "core/io/stream.h"
#include "core/memory/ref.h"
#include "core/exception/exception.h"
#include "core/preprocessor/api.h"

namespace wk
{
	class WORKSHOP_API BaseMemoryStream : public Stream
	{
	public:
		BaseMemoryStream() = default;
		virtual ~BaseMemoryStream() = default;

	public:
		void* data() override
		{
			return const_cast<void*>(const_cast<const BaseMemoryStream*>(this)->data_ref());
		};

		std::size_t length() const override { return m_length; };

		std::size_t position() const override { return m_position; };

		std::size_t seek(std::size_t position, SeekMode mode = SeekMode::Set) override;

		bool is_open() const override
		{
			return data_ref() != nullptr && m_length > 0;
		};

		bool is_readable() const override
		{
			return data_ref() != nullptr && m_length > 0;
		};

		bool is_writable() const override
		{
			return data_ref() != nullptr && m_length > 0;
		};

	public:
		virtual void* data_ref() const = 0;

	protected:
		std::size_t read_data(void* ptr, std::size_t length) override;
		std::size_t write_data(const void* ptr, std::size_t length) override;

	protected:
		std::size_t m_length = 0;
		std::size_t m_position = 0;
	};
}
