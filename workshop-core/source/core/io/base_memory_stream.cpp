#include "base_memory_stream.h"

#include "core/memory/memory.h"

namespace wk
{
	std::size_t BaseMemoryStream::seek(std::size_t position, SeekMode mode)
	{
		switch (mode)
		{
		case SeekMode::Set:
			m_position = position;
			break;

		case SeekMode::Add:
			m_position += position;
			break;

		default:
			break;
		}

		m_position = std::min(m_position, length());
		return m_position;
	};

	std::size_t BaseMemoryStream::read_data(void* ptr, std::size_t length)
	{
		if (data() == nullptr || ptr == nullptr || length <= 0 || m_length <= 0 || m_position >= m_length)
			return 0;

		if (length > m_length - m_position)
			length = m_length - m_position;

		Memory::copy((uint8_t*)data() + m_position, ptr, length);
		m_position += length;

		return length;
	};

	std::size_t BaseMemoryStream::write_data(const void* ptr, std::size_t length)
	{
		if (data() == nullptr || ptr == nullptr || length <= 0 || m_length <= 0 || m_position >= m_length)
			return 0;

		if (length > m_length - m_position)
			length = m_length - m_position;

		Memory::copy(ptr, (uint8_t*)data() + m_position, length);
		m_position += length;

		return length;
	};
}
