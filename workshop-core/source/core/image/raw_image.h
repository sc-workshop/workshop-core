#pragma once

#include "core/image/image.h"
#include "core/preprocessor/api.h"
#include "core/math/color_rgba.h"
#include "core/memory/ref.h"
#include "core/math/rect.h"
#include "core/io/memory_stream.h"

namespace wk
{
	class RawImage;
	using RawImageRef = Ref<RawImage>;

	class WORKSHOP_API RawImage : public Image
	{
	public:
		RawImage(const RawImage&);

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

		RawImage(const ColorRGBA& color);

		virtual ~RawImage() = default;

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

		void extract_channel(RawImageRef& output, uint8_t channel) const;
		Image::T* at(Image::SizeT x, Image::SizeT y) const;

		template<typename T>
		T& at(Image::SizeT x, Image::SizeT y) const
		{
			return *reinterpret_cast<T*>(at(x, y));
		}

		Image::Bound bound() const;

		RawImageRef crop(const Image::Bound& bound) const;

	private:
		Image::BasePixelType m_type;
		Image::ColorSpace m_space;
		Image::PixelDepth m_depth;

		std::uint8_t* m_data = nullptr;
		wk::Ref<MemoryStream> m_allocated_data;
	};
}