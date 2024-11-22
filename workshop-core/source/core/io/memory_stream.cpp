#include "core/io/memory_stream.h"

#include "core/memory/memory.h"

namespace wk
{
	MemoryStream::MemoryStream(std::size_t length) : m_allocated_data(Memory::allocate(length))
	{
		m_data = m_allocated_data;
		m_length = length;
	}

	MemoryStream::MemoryStream(std::uint8_t* data, std::size_t length)
	{
		m_data = data;
		m_length = length;
	}

	MemoryStream::~MemoryStream()
	{
		if (m_allocated_data)
		{
			Memory::free(reinterpret_cast<void*>(m_allocated_data));
		}
	};

	std::size_t MemoryStream::seek(std::size_t position, SeekMode mode)
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

		return m_position;
	};

	std::size_t MemoryStream::read_data(void* ptr, std::size_t length)
	{
		if (m_data == nullptr || ptr == nullptr || length <= 0 || m_length <= 0 || m_position >= m_length)
			return 0;

		if (length > m_length - m_position)
			length = m_length - m_position;

		Memory::copy(m_data + m_position, ptr, length);
		m_position += length;

		return length;
	};

	std::size_t MemoryStream::write_data(const void* ptr, std::size_t length)
	{
		if (m_data == nullptr || ptr == nullptr || length <= 0 || m_length <= 0 || m_position >= m_length)
			return 0;

		if (length > m_length - m_position)
			length = m_length - m_position;

		Memory::copy(ptr, m_data + m_position, length);
		m_position += length;

		return length;
	};
}
