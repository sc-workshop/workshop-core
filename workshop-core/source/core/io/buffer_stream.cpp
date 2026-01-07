#include "core/io/buffer_stream.h"

#include "core/memory/memory.h"

namespace wk
{
	std::size_t BufferStream::seek(std::size_t position, SeekMode mode)
	{
		switch (mode)
		{
		case SeekMode::Set:
			m_position = position > m_buffer.size() ? m_buffer.size() : position;
			break;

		case SeekMode::Add:
			m_position = m_position + position > length() ? length() : m_position + position;
			break;

		default:
			break;
		}

		return m_position;
	};

	std::size_t BufferStream::read_data(void* ptr, std::size_t length)
	{
		if (length == 0 || m_position >= this->length())
		{
			return 0;
		}

		std::size_t result = this->length() - m_position;

		if (result > length)
		{
			result = length;
		}

		Memory::copy(m_buffer.data() + m_position, ptr, result);

		m_position += result;

		return result;
	};

	std::size_t BufferStream::write_data(const void* ptr, std::size_t length)
	{
		if (m_position + length > m_buffer.size()) {
			std::size_t old_size = m_buffer.size();
			m_buffer.resize(old_size + length);
		}

		Memory::copy(ptr, &m_buffer[m_position], length);

		m_position += length;
		return length;
	};
}
