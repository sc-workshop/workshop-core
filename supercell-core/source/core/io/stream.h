#pragma once

#include "core/io/endian.h"
#include "core/generic/non_copyable.h"
#include "core/preprocessor/api.h"

#include <string>

namespace sc
{
	class SUPERCELL_API Stream
	{
	public:
		NON_COPYABLE(Stream)

	public:
		enum class SeekMode : std::uint8_t
		{
			Set = 0,
			Add,
		};

	public:
		Stream() = default;
		virtual ~Stream() = default;

	public:
		virtual void* data() = 0;

		virtual std::size_t length() const = 0;

		virtual std::size_t position() const = 0;
		virtual std::size_t seek(std::size_t position, SeekMode mode = SeekMode::Set) = 0;

		virtual bool is_open() const = 0;
		virtual bool is_readable() const = 0;
		virtual bool is_writable() const = 0;

	public:
		std::size_t read(void* buffer, std::size_t length);

		bool read_bool();

		std::int8_t read_byte();

		std::uint8_t read_unsigned_byte();

		std::int16_t read_short(Endian endian = Endian::Little);

		std::uint16_t read_unsigned_short(Endian endian = Endian::Little);

		std::int32_t read_int(Endian endian = Endian::Little);

		std::uint32_t read_unsigned_int(Endian endian = Endian::Little);

		std::int64_t read_long(Endian endian = Endian::Little);

		std::uint64_t read_unsigned_long(Endian endian = Endian::Little);

		float read_float(Endian endian = Endian::Little);

		double read_double(Endian endian = Endian::Little);

		std::string read_string(int32_t length = 0);

	public:
		std::size_t write(const void* buffer, std::size_t length);

		std::size_t write_bool(bool value);

		std::size_t write_byte(std::int8_t value);

		std::size_t write_unsigned_byte(std::uint8_t value);

		std::size_t write_short(std::int16_t value, Endian endian = Endian::Little);

		std::size_t write_unsigned_short(std::uint16_t value, Endian endian = Endian::Little);

		std::size_t write_int(std::int32_t value, Endian endian = Endian::Little);

		std::size_t write_unsigned_int(std::uint32_t value, Endian endian = Endian::Little);

		std::size_t write_long(std::int64_t value, Endian endian = Endian::Little);

		std::size_t write_unsigned_long(std::uint64_t value, Endian endian = Endian::Little);

		std::size_t write_float(float value, Endian endian = Endian::Little);

		std::size_t write_double(double value, Endian endian = Endian::Little);

		std::size_t write_string(std::string str);

	protected:
		virtual std::size_t read_data(void* buffer, std::size_t length) = 0;
		virtual std::size_t write_data(const void* buffer, std::size_t length) = 0;
	};
}