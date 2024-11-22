#pragma once

#include "core/io/stream.h"
#include "core/preprocessor/api.h"

#include <vector>

namespace wk
{
	class BufferStream : public Stream
	{
	public:
		NON_COPYABLE(BufferStream)

	public:
		BufferStream() = default;

		BufferStream(std::size_t capacity)
		{
			m_buffer.resize(capacity);
		};

		virtual ~BufferStream() = default;

	public:
		void* data() const { return (void*)m_buffer.data(); };

		void* data() override
		{
			return const_cast<void*>(const_cast<const BufferStream*>(this)->data());
		};

		std::size_t length() const override { return m_buffer.size(); };

		std::size_t position() const override { return m_position; };

		std::size_t seek(std::size_t position, SeekMode mode = SeekMode::Set) override;

		bool is_open() const override { return true; };

		bool is_readable() const override { return true; };

		bool is_writable() const override { return true; };

	public:
		void resize(std::size_t length)
		{
			m_buffer.resize(length);
		};

		void clear()
		{
			m_buffer.clear();
			m_position = 0;
		};

	protected:
		std::size_t read_data(void* ptr, std::size_t length) override;

		std::size_t write_data(const void* ptr, std::size_t length) override;

	private:
		std::vector<std::uint8_t> m_buffer;
		std::size_t m_position = 0;
	};
}
