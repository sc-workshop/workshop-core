#pragma once

#include "core/io/endian.h"
#include "core/generic/non_copyable.h"
#include "core/preprocessor/api.h"

#include <string>
#include <cstdint>

namespace wk
{
	class WORKSHOP_API Stream
	{
	public:
		NON_COPYABLE(Stream)

	public:
		enum class SeekMode : uint8_t
		{
			Set = 0,
			Add,
		};

	public:
		Stream() = default;
		virtual ~Stream() = default;

	public:
		virtual void* data() = 0;

		virtual size_t length() const = 0;

		virtual size_t position() const = 0;
		virtual size_t seek(size_t position, SeekMode mode = SeekMode::Set) = 0;

		virtual bool is_open() const = 0;
		virtual bool is_readable() const = 0;
		virtual bool is_writable() const = 0;

	public:
		size_t read(void* buffer, size_t length);

		bool read_bool();

		std::int8_t read_byte();

		uint8_t read_unsigned_byte();

		int16_t read_short(Endian endian = Endian::Little);

		std::uint16_t read_unsigned_short(Endian endian = Endian::Little);

		int32_t read_int(Endian endian = Endian::Little);

		std::uint32_t read_unsigned_int(Endian endian = Endian::Little);

		std::int64_t read_long(Endian endian = Endian::Little);

		std::uint64_t read_unsigned_long(Endian endian = Endian::Little);

		float read_float(Endian endian = Endian::Little);

		double read_double(Endian endian = Endian::Little);

		std::string read_string(int32_t length = 0);

	public:
		size_t write(const void* buffer, size_t length);

		size_t write_bool(bool value);

		size_t write_byte(std::int8_t value);

		size_t write_unsigned_byte(uint8_t value);

		size_t write_short(int16_t value, Endian endian = Endian::Little);

		size_t write_unsigned_short(std::uint16_t value, Endian endian = Endian::Little);

		size_t write_int(int32_t value, Endian endian = Endian::Little);

		size_t write_unsigned_int(std::uint32_t value, Endian endian = Endian::Little);

		size_t write_long(std::int64_t value, Endian endian = Endian::Little);

		size_t write_unsigned_long(std::uint64_t value, Endian endian = Endian::Little);

		size_t write_float(float value, Endian endian = Endian::Little);

		size_t write_double(double value, Endian endian = Endian::Little);

		size_t write_string(std::string str);

	protected:
		virtual size_t read_data(void* buffer, size_t length) = 0;
		virtual size_t write_data(const void* buffer, size_t length) = 0;
	};
}