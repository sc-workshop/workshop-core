#include "core/image/raw_image.h"
#include <array>

namespace wk
{
	RawImage::RawImage(const RawImage& other)
		: RawImage(other.width(), other.height(), other.depth(), other.colorspace())
	{
		other.copy(*this);
	}

	RawImage::RawImage(
		uint8_t* data,
		uint16_t width, uint16_t height,
		Image::PixelDepth depth,
		Image::ColorSpace space
	) : m_space(space), m_depth(depth), m_data(data)
	{
		m_width = width;
		m_height = height;
		m_type = Image::PixelDepthBaseTypeTable[(uint8_t)depth];
	};

	RawImage::RawImage(
		uint16_t width, uint16_t height,
		Image::PixelDepth depth, Image::ColorSpace space
	) : m_space(space), m_depth(depth)
	{
		m_width = width;
		m_height = height;
		m_type = Image::PixelDepthBaseTypeTable[(uint8_t)depth];

		size_t data_length = Image::calculate_image_length(width, height, depth);
		m_allocated_data = wk::CreateRef<MemoryStream>(data_length);
		m_data = (uint8_t*)m_allocated_data->data();

		memset(m_allocated_data->data(), 0, data_length);
	};

	RawImage::RawImage(const ColorRGBA& color) : 
		RawImage((uint8_t*)& color, 1, 1, Image::PixelDepth::RGBA8, Image::ColorSpace::Linear)
	{
	}

	size_t RawImage::data_length() const
	{
		return Image::calculate_image_length(m_width, m_height, m_depth);
	};

	uint8_t* RawImage::data() const
	{
		return m_data;
	};

	Image::BasePixelType RawImage::base_type() const
	{
		return m_type;
	};

	Image::ColorSpace RawImage::colorspace() const
	{
		return m_space;
	}

	Image::PixelDepth RawImage::depth() const
	{
		return m_depth;
	}

	void RawImage::copy(RawImage& image) const
	{
		size_t data_size = Image::calculate_image_length(image.width(), image.height(), image.depth());
		wk::Ref<MemoryStream> final_buffer;

		Image::PixelDepth current_depth = m_depth;
		if (image.width() != m_width || image.height() != m_height)
		{
			// Convert image to a more convenient type to change size
			switch (m_depth)
			{
				// RGBA
			case Image::PixelDepth::RGBA4:
			case Image::PixelDepth::RGB5_A1:
				current_depth = Image::PixelDepth::RGBA8;
				break;

				// RGB
			case Image::PixelDepth::RGB565:
				current_depth = Image::PixelDepth::RGB8;
				break;

			case Image::PixelDepth::RGB8:
			case Image::PixelDepth::LUMINANCE8_ALPHA8:
			case Image::PixelDepth::LUMINANCE8:
			case Image::PixelDepth::RGBA8:
			default:
				break;
			}

			wk::Ref<MemoryStream> depth_buffer;
			if (current_depth != m_depth)
			{
				depth_buffer = wk::CreateRef<MemoryStream>(Image::calculate_image_length(m_width, m_height, current_depth));
				Image::remap(m_data, (uint8_t*)depth_buffer->data(), m_width, m_height, m_depth, current_depth);
			}

			final_buffer = CreateRef<MemoryStream>(Image::calculate_image_length(image.width(), image.height(), m_depth));
			Image::resize(
				depth_buffer ? (uint8_t*)depth_buffer->data() : m_data, (uint8_t*)final_buffer->data(),
				m_width, m_height,
				image.width(), image.height(),
				m_type, m_space
			);
		}

		if (image.depth() != m_depth)
		{
			Image::remap(
				final_buffer ? (uint8_t*)final_buffer->data() : m_data, image.data(),
				image.width(), image.height(),
				current_depth, image.depth()
			);
			return;
		}

		Memory::copy(
			final_buffer ? (uint8_t*)final_buffer->data() : m_data, image.data(), data_size
		);
	};

	void RawImage::write(Stream& buffer)
	{
		buffer.write(m_data, data_length());
	};

	Image::T* RawImage::at(Image::SizeT x, Image::SizeT y) const
	{
		return m_data + (y * m_width + x) * PixelDepthTable[(std::uint16_t)m_depth].byte_count;
	};

	void RawImage::extract_channel(RawImageRef& output, uint8_t channel) const
	{
		if (is_complex()) return;

		output = CreateRef<RawImage>(
			m_width, m_height,
			Image::PixelDepth::LUMINANCE8,
			m_space
		);

		for (Image::SizeT y = 0; m_height > y; y++)
		{
			for (Image::SizeT x = 0; m_width > x; x++)
			{
				Image::T* pixel = at(x, y);
				Image::T* output_pixel = output->at(x, y);
				*output_pixel = pixel[channel];
			}
		}
	};

	Image::Bound RawImage::bound() const
	{
		if (base_type() != Image::BasePixelType::L)
		{
			throw wk::Exception("Invalid image type");
		}

		Image::Bound result(
			width(), 0,
			0, height()
		);

		bool valid = false;
		for (Image::SizeT y = 0; m_height > y; y++)
		{
			for (Image::SizeT x = 0; m_width > x; x++)
			{
				if (0 >= at<uint8_t>(x, y)) continue;

				result.left = std::min<uint16_t>(result.left, x);
				result.right = std::max<uint16_t>(result.right, x);
				result.bottom = std::min<uint16_t>(result.bottom, y);
				result.top = std::max<uint16_t>(result.top, y);

				valid = true;
			}
		}

		if (!valid) return Image::Bound();
		return Image::Bound(
			result.left, result.bottom,
			(uint16_t)std::abs(result.left - result.right) + 1, 
			(uint16_t)std::abs(result.bottom - result.top) + 1
		);
	}

	RawImageRef RawImage::crop(const Image::Bound& bound) const
	{
		RawImageRef result = CreateRef<RawImage>(
			bound.width, bound.height,
			m_depth, m_space
		);

		for (uint16_t y = 0; bound.height > y; y++)
		{
			for (uint16_t x = 0; bound.width > x; x++)
			{
				Image::T* pixel = at(bound.x + x, bound.y + y);
				Image::T* output_pixel = result->at(x, y);
				Memory::copy<Image::T>(pixel, output_pixel, PixelDepthTable[(std::uint16_t)m_depth].byte_count);
			}
		}

		return result;
	}
}