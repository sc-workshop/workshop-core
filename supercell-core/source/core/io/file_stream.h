#pragma once

#include "core/io/stream.h"
#include "core/memory/memory.h"
#include "core/exception/exception.h"
#include "core/preprocessor/api.h"

#include "core/generic/non_copyable.h"

#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace sc
{
	class SUPERCELL_API InputFileStream : public Stream
	{
	public:
		NON_COPYABLE(InputFileStream)

	public:
		InputFileStream(const fs::path& path)
		{
			if (!fs::exists(path))
			{
				std::string path_string(path.u8string()); // scary
				throw Exception("File %s doesn't exist!", path_string.c_str());
			}

			m_file.open(path.c_str(), std::ios_base::binary);

			m_file.seekg(0, std::ios::end);
			m_file_size = static_cast<std::size_t>(m_file.tellg());
			m_file.seekg(0);
		};

		virtual ~InputFileStream()
		{
			if (m_data)
			{
				Memory::free(m_data);
			}
		};

	public:
		void* data() override
		{
			size_t saved_position = position();

			m_data = Memory::allocate(m_file_size);

			m_file.seekg(0);
			m_file.read((char*)m_data, m_file_size);
			m_file.seekg(saved_position);

			return m_data;
		};

		size_t length() const override
		{
			return m_file_size;
		};

		size_t position() const override
		{
			return m_position;
		};

		size_t seek(size_t position, SeekMode mode = SeekMode::Set) override
		{
			switch (mode)
			{
			case SeekMode::Set:
				m_file.seekg(position, std::ios::beg);
				m_position = position;
				break;

			case SeekMode::Add:
				m_file.seekg(position, std::ios::cur);
				m_position += position;
				break;

			default:
				break;
			}

			return this->position();
		};

		bool is_open() const override
		{
			return true;
		};

		bool is_readable() const override
		{
			return true;
		};

		bool is_writable() const override
		{
			return false;
		};

	protected:
		size_t read_data(void* ptr, size_t length) override
		{
			size_t result = (position() + length) > m_file_size ? m_file_size - position() : length;

			// TODO(pavidloq): remove c style cast?
			if (!m_data)
			{
				m_file.read((char*)ptr, result);
			}
			else
			{
				Memory::copy<std::uint8_t>((uint8_t*)m_data + position(), ptr, result);
			}

			m_position += result;
			return result;
		};

		size_t write_data(const void*, size_t) override
		{
			throw Exception("Cannot write data to InputFileStream!");
		};

	private:
		mutable std::ifstream m_file;

		size_t m_file_size;
		size_t m_position = 0;

		void* m_data = nullptr;
	};

	class OutputFileStream : public Stream
	{
	public:
		NON_COPYABLE(OutputFileStream)

	public:
		OutputFileStream(const fs::path& path)
		{
			m_file.open(path, std::ios_base::binary);

			m_file.seekp(0, std::ios::end);
			m_file_size = static_cast<size_t>(m_file.tellp());
			m_file.seekp(0);
		};

		virtual ~OutputFileStream() = default;

	public:
		void* data() override
		{
			throw Exception("Cannot get data from OutputFileStream!");
		};

		size_t length() const override
		{
			return m_file_size;
		};

		size_t position() const override
		{
			return m_file.tellp();
		};

		size_t seek(size_t position, SeekMode mode) override
		{
			switch (mode)
			{
			case SeekMode::Set:
				m_file.seekp(position, std::ios::beg);
				break;

			case SeekMode::Add:
				m_file.seekp(position, std::ios::cur);
				break;

			default:
				break;
			}

			return this->position();
		};

		bool is_open() const override
		{
			return true;
		};

		bool is_readable() const override
		{
			return false;
		};

		bool is_writable() const override
		{
			return true;
		};

	protected:
		size_t read_data(void*, size_t) override
		{
			throw Exception("Cannot read data from OutputFileStream!");
		};

		size_t write_data(const void* ptr, size_t length) override
		{
			size_t pos = m_file.tellp();

			// TODO(pavidloq): remove c style cast?
			m_file.write((char*)(ptr), length);

			const size_t result = (size_t)m_file.tellp() - pos;
			m_file_size += result;

			return result;
		};

	private:
		mutable std::ofstream m_file;
		size_t m_file_size;
	};
}