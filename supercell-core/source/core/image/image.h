#pragma once

#include "core/io/stream.h"
#include "core/preprocessor/api.h"

namespace sc
{
	class SUPERCELL_API Image
	{
	public:
		Image() = default;
		virtual ~Image() = default;

	public:
		enum class BasePixelType : std::uint8_t
		{
			RGBA = 4,
			RGB = 3,
			LA = 2,
			L = 1,
		};

		enum class ColorSpace : std::uint8_t
		{
			Linear = 0,
			sRGB,
		};

		struct PixelDepthInfo
		{
			std::uint8_t byte_count;
			std::uint8_t r_bits;
			std::uint8_t g_bits;
			std::uint8_t b_bits;
			std::uint8_t a_bits;
		};

		static const PixelDepthInfo PixelDepthTable[];

		static const BasePixelType PixelDepthBaseTypeTable[];

		enum class PixelDepth : std::uint16_t
		{
			RGBA8 = 0,
			RGBA4,
			RGB5_A1,
			RGB8,
			RGB565,
			LUMINANCE8_ALPHA8,
			LUMINANCE8
		};

	public:
		static size_t calculate_image_length(std::uint16_t width, std::uint16_t height, PixelDepth depth);

		static bool check_depth_base_type(BasePixelType type, PixelDepth depth);

		static void resize(
			std::uint8_t* input_data, std::uint8_t* output_data,
			std::uint16_t width, std::uint16_t height,
			std::uint16_t new_width, std::uint16_t new_height,
			BasePixelType type, ColorSpace space,
			bool premultiply = true
		);

		static void remap(
			std::uint8_t* input_data, std::uint8_t* output_data,
			std::uint16_t width, std::uint16_t height,
			PixelDepth source, PixelDepth destination
		);

	public:
		virtual void write(Stream& buffer) = 0;

	public:
		virtual std::uint16_t width() const;
		virtual std::uint16_t height() const;

		virtual BasePixelType base_type() const = 0;
		virtual ColorSpace colorspace() const = 0;
		virtual PixelDepth depth() const = 0;

		virtual std::size_t data_length() const = 0;
		virtual std::uint8_t* data() const = 0;

		virtual bool is_compressed() const = 0;

	protected:
		std::uint16_t m_width = 0;
		std::uint16_t m_height = 0;
	};
}