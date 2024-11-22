#pragma once

#include "core/io/stream.h"
#include "core/exception/exception.h"
#include "core/preprocessor/api.h"

namespace wk
{
	class WORKSHOP_API MemoryStream : public Stream
	{
	public:
		NON_COPYABLE(MemoryStream)

	public:
		MemoryStream(std::size_t length);
		MemoryStream(std::uint8_t* data, std::size_t length);

		virtual ~MemoryStream();

	public:
		void* data() const { return (void*)m_data; };

		void* data() override
		{
			return const_cast<void*>(const_cast<const MemoryStream*>(this)->data());
		};

		std::size_t length() const override { return m_length; };

		std::size_t position() const override { return m_position; };

		std::size_t seek(std::size_t position, SeekMode mode = SeekMode::Set) override;

		bool is_open() const override
		{
			return m_data != nullptr && m_length > 0;
		};

		bool is_readable() const override
		{
			return m_data != nullptr && m_length > 0;
		};

		bool is_writable() const override
		{
			return m_data != nullptr && m_length > 0;
		};

	protected:
		std::size_t read_data(void* ptr, std::size_t length) override;
		std::size_t write_data(const void* ptr, std::size_t length) override;

	private:
		std::uint8_t* m_data;
		std::uint8_t* const m_allocated_data = nullptr;

		std::size_t m_length = 0;
		std::size_t m_position = 0;
	};
}
