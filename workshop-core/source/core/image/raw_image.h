#pragma once

#include "core/image/image.h"
#include "core/preprocessor/api.h"

namespace wk
{
	class WORKSHOP_API RawImage : public Image
	{
	public:
		RawImage(const RawImage&) = delete;

		RawImage(
			std::uint8_t* data,
			std::uint16_t width, std::uint16_t height,
			Image::PixelDepth depth = Image::PixelDepth::RGBA8,
			Image::ColorSpace space = Image::ColorSpace::Linear
		);

		RawImage(
			std::uint16_t width, std::uint16_t height,
			Image::PixelDepth depth,
			Image::ColorSpace space = Image::ColorSpace::Linear
		);

		virtual ~RawImage();

	public:
		virtual std::size_t data_length() const;
		virtual std::uint8_t* data() const;

		virtual BasePixelType base_type() const;
		virtual ColorSpace colorspace() const;
		virtual PixelDepth depth() const;

		virtual bool is_compressed() const { return false; };

	public:
		void copy(RawImage& image) const;

		void write(Stream& buffer);

	private:
		Image::BasePixelType m_type;
		Image::ColorSpace m_space;
		Image::PixelDepth m_depth;

		std::uint8_t* m_data = nullptr;
		std::uint8_t* m_allocated_data = nullptr;
	};
}