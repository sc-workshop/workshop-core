#pragma once

#include "core/io/stream.h"
#include "core/preprocessor/api.h"
#include "core/math/point.h"
#include "core/math/rect.h"
#include "core/hashing/ncrypto/xxhash.h"

#include <array>

namespace wk
{
	class WORKSHOP_API Image
	{
	public:
		using T = uint8_t;
		using SizeT = uint16_t;
		using Size = Point_t<SizeT>;
		using Bound = Rect_t<SizeT>;

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
			std::array<uint8_t, 4> order;
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
		static size_t calculate_image_length(SizeT width, SizeT height, PixelDepth depth);

		static bool check_depth_base_type(BasePixelType type, PixelDepth depth);

		static void resize(
			std::uint8_t* input_data, std::uint8_t* output_data,
			SizeT width, SizeT height,
			SizeT new_width, SizeT new_height,
			BasePixelType type, ColorSpace space,
			bool premultiply = true
		);

		static void remap(
			std::uint8_t* input_data, std::uint8_t* output_data,
			SizeT width, SizeT height,
			PixelDepth source, PixelDepth destination
		);

	public:
		virtual void write(Stream& buffer) = 0;

	public:
		virtual SizeT width() const;
		virtual SizeT height() const;

		virtual BasePixelType base_type() const = 0;
		virtual ColorSpace colorspace() const = 0;
		virtual PixelDepth depth() const = 0;

		virtual std::size_t data_length() const = 0;
		virtual T* data() const = 0;

		virtual bool is_compressed() const = 0;

		virtual size_t hash() const;

	public:
		uint8_t channels() const;
		bool is_complex() const;
		Image::Size size() const;
		size_t pixel_size() const;

	protected:
		SizeT m_width = 0;
		SizeT m_height = 0;
	};

	namespace hash
	{
		template<>
		struct Hash_t<Image>
		{
			template<typename T>
			static void update(wk::hash::HashStream<T>& stream, const Image& image)
			{
				stream.update(image.depth());
				stream.update(image.colorspace());
				stream.update(image.width());
				stream.update(image.height());
				stream.update(image.data(), image.data_length());
			}
		};
	}
}