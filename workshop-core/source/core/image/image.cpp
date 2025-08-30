#include "core/image/image.h"
#include "core/memory/memory.h"
#include "core/stb/stb.h"

#include <vector>
#include <map>

namespace wk
{
#pragma region Constants
	const Image::PixelDepthInfo Image::PixelDepthTable[] =
	{
		{{0, 1, 2, 3}, 4, 8, 8, 8, 8},			//RGBA8
		{{3, 2, 1, 0}, 2, 4, 4, 4, 4},			//RGBA4
		{{3, 2, 1, 0}, 2, 5, 5, 5, 1},			//RGB5_A1
		{{0, 1, 2, 0xFF}, 3, 8, 8, 8, 0},		//RGB8
		{{3, 2, 1, 0xFF}, 2, 5, 6, 5, 0},		//RGB565
		{{0, 0xFF, 0xFF, 3}, 2, 8, 0, 0, 8},			//LUMINANCE8_ALPHA8
		{{0, 0xFF, 0xFF, 0xFF}, 1, 8, 0, 0, 0},		//LUMINANCE8
	};

	const Image::BasePixelType Image::PixelDepthBaseTypeTable[] =
	{
		Image::BasePixelType::RGBA, //RGBA8
		Image::BasePixelType::RGBA,	//RGBA4
		Image::BasePixelType::RGBA,	//RGB5_A1
		Image::BasePixelType::RGB,	//RGB8
		Image::BasePixelType::RGB,	//RGB565
		Image::BasePixelType::LA,	//LUMINANCE8_ALPHA8
		Image::BasePixelType::L,	//LUMINANCE8
	};
#pragma endregion

#pragma region Static Functions
	//			<---------------- Static members ---------------->
	std::size_t Image::calculate_image_length(SizeT width, SizeT height, PixelDepth depth)
	{
		size_t pixel_size = PixelDepthTable[(std::uint16_t)depth].byte_count;
		return ((size_t)width * height) * pixel_size;
	}

	bool Image::check_depth_base_type(Image::BasePixelType type, Image::PixelDepth depth)
	{
		Image::BasePixelType depth_base_type = Image::PixelDepthBaseTypeTable[(std::uint16_t)depth];

		return depth_base_type == type;
	}
#pragma endregion

	void Image::resize(
		std::uint8_t* input_data, std::uint8_t* output_data,
		SizeT width, SizeT height,
		SizeT new_width, SizeT new_height,
		BasePixelType type, ColorSpace space,
		bool premultiply
	)
	{
		bool has_alpha = type == BasePixelType::LA || type == BasePixelType::RGBA;

		stbir_resize_uint8_generic(
			input_data, width, height, 0,
			output_data, new_width, new_height, 0,
			(std::uint8_t)type, has_alpha ? (std::uint8_t)type - 1 : -1, premultiply ? STBIR_FLAG_ALPHA_PREMULTIPLIED : 0, STBIR_EDGE_CLAMP,
			STBIR_FILTER_DEFAULT, space == Image::ColorSpace::Linear ? STBIR_COLORSPACE_LINEAR : STBIR_COLORSPACE_SRGB, NULL
		);
	};

	void Image::remap(
		std::uint8_t* input_data, std::uint8_t* output_data,
		SizeT width, SizeT height,
		Image::PixelDepth source, Image::PixelDepth destination
	)
	{
		size_t pixel_count = (size_t)width * height;

		const PixelDepthInfo& input_pixel_info = PixelDepthTable[(std::uint16_t)source];
		const PixelDepthInfo& output_pixel_info = PixelDepthTable[(std::uint16_t)destination];

		std::uint32_t input_pixel_buffer = 0;
		std::uint32_t output_pixel_buffer = 0;
		std::uint8_t* input_pixel_data = nullptr;
		std::uint8_t* output_pixel_data = nullptr;

		float r_channel = 0.f;
		float g_channel = 0.f;
		float b_channel = 0.f;
		float a_channel = 0.f;

		struct Channel
		{
			float& value;
			const uint64_t input_mask;
			const uint8_t& input_bits;

			const uint64_t output_mask;
			const uint8_t& output_bits;

			const float& default_value;
		};

#define MASK(value) (uint64_t)pow(2, value) - 1

		const std::vector<Channel> _channels =
		{
			{r_channel, MASK(input_pixel_info.r_bits), input_pixel_info.r_bits, MASK(output_pixel_info.r_bits), output_pixel_info.r_bits, 0.f},
			{g_channel, MASK(input_pixel_info.g_bits), input_pixel_info.g_bits, MASK(output_pixel_info.g_bits), output_pixel_info.g_bits, r_channel},
			{b_channel, MASK(input_pixel_info.b_bits), input_pixel_info.b_bits, MASK(output_pixel_info.b_bits), output_pixel_info.b_bits, b_channel},
			{a_channel, MASK(input_pixel_info.a_bits), input_pixel_info.a_bits, MASK(output_pixel_info.a_bits), output_pixel_info.a_bits, 1.f},
		};

#undef MASK

		for (std::uint64_t pixel_index = 0; pixel_count > pixel_index; pixel_index++)
		{
			output_pixel_buffer = 0;

			input_pixel_data = input_data + (pixel_index * input_pixel_info.byte_count);
			output_pixel_data = output_data + (pixel_index * output_pixel_info.byte_count);

			Memory::copy<std::uint8_t>(
				input_pixel_data,
				(std::uint8_t*)(&input_pixel_buffer),
				input_pixel_info.byte_count
			);

			// Pixel Decoding
			{
				uint8_t bit_index = 0;
				uint8_t channel_index = 0;

				for (const uint8_t& ordered_index : input_pixel_info.order)
				{
					const Channel& channel = _channels[ordered_index == 0xFF ? channel_index : ordered_index];

					if (channel.input_bits)
					{
						uint8_t value = static_cast<std::uint8_t>(((channel.input_mask << bit_index) & input_pixel_buffer) >> bit_index);
						channel.value = (float)value / channel.input_mask;

						bit_index += channel.input_bits;
					}
					else
					{
						channel.value = channel.default_value;
					}

					channel_index++;
				}
			}

			// Some Pixel Type Specific actions

			switch (source)
			{
			case Image::PixelDepth::LUMINANCE8_ALPHA8:
			case Image::PixelDepth::LUMINANCE8:
				b_channel = g_channel = r_channel;
				break;
			default:
				break;
			}

			switch (destination)
			{
			case Image::PixelDepth::LUMINANCE8_ALPHA8:
			case Image::PixelDepth::LUMINANCE8:
				r_channel = (r_channel + g_channel + b_channel) / 3;
				break;
			default:
				break;
			}

			// Pixel Encoding
			{
				std::uint8_t bit_offset = 0;

				for (const uint8_t& channel_index : output_pixel_info.order)
				{
					if (channel_index >= 0xFF) continue;
					
					const Channel& channel = _channels[channel_index];

					if (channel.output_bits)
					{
						output_pixel_buffer |= (((uint8_t)(channel.value * channel.output_mask) & channel.output_mask) << bit_offset);
						bit_offset += channel.output_bits;
					}
				}
			}

			Memory::copy<std::uint8_t>(
				(std::uint8_t*)&output_pixel_buffer,
				output_pixel_data,
				output_pixel_info.byte_count
			);
		}
	}

	Image::SizeT Image::width() const
	{
		return m_width;
	};

	Image::SizeT Image::height() const
	{
		return m_height;
	};

	uint8_t Image::channels() const
	{
		switch (base_type())
		{
		case BasePixelType::RGBA:
			return 4;
		case BasePixelType::RGB:
			return 3;
		case BasePixelType::LA:
			return 2;
		case BasePixelType::L:
			return 1;
		default:
			throw Exception();
		}
	}

	bool Image::is_complex() const
	{
		switch (depth())
		{
		case PixelDepth::RGBA8:
		case PixelDepth::RGB8:
		case PixelDepth::LUMINANCE8_ALPHA8:
		case PixelDepth::LUMINANCE8:
			return false;
		default:
			return true;
		}
	}

	size_t Image::hash() const
	{
		wk::hash::XxHash hash;
		hash.update(*this);
	
		return hash.digest();
	}

	Image::Size Image::size() const
	{
		return { m_width, m_height };
	}

	size_t Image::pixel_size() const
	{
		return Image::PixelDepthTable[(uint16_t)depth()].byte_count;
	}
}