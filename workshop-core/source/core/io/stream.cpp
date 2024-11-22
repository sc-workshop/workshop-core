#include "core/io/stream.h"

namespace wk
{
	std::size_t Stream::read(void* buffer, std::size_t length)
	{
		return read_data(buffer, length);
	};

	bool Stream::read_bool()
	{
		bool value; read_data(&value, sizeof(bool));
		return value;
	};

	std::int8_t Stream::read_byte()
	{
		int8_t value; read_data(&value, sizeof(std::int8_t));
		return value;
	};

	std::uint8_t Stream::read_unsigned_byte()
	{
		std::uint8_t value; read_data(&value, sizeof(std::uint8_t));
		return value;
	};

	std::int16_t Stream::read_short(Endian endian)
	{
		int16_t value; read_data(&value, sizeof(std::int16_t));
		if (!is_endian(endian)) swap_endian(&value);
		return value;
	};

	std::uint16_t Stream::read_unsigned_short(Endian endian)
	{
		std::uint16_t value; read_data(&value, sizeof(std::uint16_t));
		if (!is_endian(endian)) swap_endian(&value);
		return value;
	};

	std::int32_t Stream::read_int(Endian endian)
	{
		int32_t value; read_data(&value, sizeof(std::int32_t));
		if (!is_endian(endian)) swap_endian(&value);
		return value;
	};

	std::uint32_t Stream::read_unsigned_int(Endian endian)
	{
		std::uint32_t value; read_data(&value, sizeof(std::uint32_t));
		if (!is_endian(endian)) value = swap_endian(value);
		return value;
	};

	std::int64_t Stream::read_long(Endian endian)
	{
		int64_t value; read_data(&value, sizeof(std::int64_t));
		if (!is_endian(endian)) swap_endian(&value);
		return value;
	};

	std::uint64_t Stream::read_unsigned_long(Endian endian)
	{
		std::uint64_t value; read_data(&value, sizeof(std::uint64_t));
		if (!is_endian(endian)) swap_endian(&value);
		return value;
	};

	float Stream::read_float(Endian endian)
	{
		float value; read_data(&value, sizeof(float));
		if (!is_endian(endian)) swap_endian(&value);
		return value;
	};

	double Stream::read_double(Endian endian)
	{
		double value; read_data(&value, sizeof(double));
		if (!is_endian(endian)) swap_endian(&value);
		return value;
	};

	std::string Stream::read_string(std::int32_t length)
	{
		std::string result;

		if (length < 0)
		{
			char sym;
			while (read(&sym, 1) && sym != '\0') result += sym;
		}
		else
		{
			for (int i = 0; i < length; i++) result += read_byte();
			result += '\0';
		}

		return result;
	};

	std::size_t Stream::write(const void* buffer, std::size_t length)
	{
		if (length)
		{
			return write_data(buffer, length);
		}
		return 0;
	};

	std::size_t Stream::write_bool(bool value)
	{
		return write_data(&value, sizeof(bool));
	};

	std::size_t Stream::write_byte(std::int8_t value)
	{
		return write_data(&value, sizeof(std::int8_t));
	};

	std::size_t Stream::write_unsigned_byte(std::uint8_t value)
	{
		return write_data(&value, sizeof(std::uint8_t));
	};

	std::size_t Stream::write_short(std::int16_t value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(std::int16_t));
	};

	std::size_t Stream::write_unsigned_short(std::uint16_t value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(std::uint16_t));
	};

	std::size_t Stream::write_int(std::int32_t value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(std::int32_t));
	};

	std::size_t Stream::write_unsigned_int(std::uint32_t value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(std::uint32_t));
	};

	std::size_t Stream::write_long(std::int64_t value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(std::int64_t));
	};

	std::size_t Stream::write_unsigned_long(std::uint64_t value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(std::uint64_t));
	};

	std::size_t Stream::write_float(float value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(float));
	};

	std::size_t Stream::write_double(double value, Endian endian)
	{
		if (!is_endian(endian)) value = swap_endian(value);
		return write_data(&value, sizeof(double));
	};

	std::size_t Stream::write_string(std::string str)
	{
		return write_data(str.c_str(), str.size() + 1);
	};
}
