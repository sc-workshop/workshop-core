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
		{4, 8, 8, 8, 8},			//RGBA8
		{2, 4, 4, 4, 4},			//RGBA4
		{2, 5, 5, 5, 1},			//RGB5_A1
		{3, 8, 8, 8, 0},			//RGB8
		{2, 5, 6, 5, 0},			//RGB565
		{2, 8, 0, 0, 8},			//LUMINANCE8_ALPHA8
		{1, 8, 0, 0, 0},			//LUMINANCE8
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

		for (std::uint64_t pixel_index = 0; pixel_count > pixel_index; pixel_index++)
		{
			std::uint32_t input_pixel_buffer = 0;
			std::uint32_t output_pixel_buffer = 0;

			std::uint8_t* input_pixel_data = input_data + (pixel_index * input_pixel_info.byte_count);
			std::uint8_t* output_pixel_data = output_data + (pixel_index * output_pixel_info.byte_count);

			Memory::copy<std::uint8_t>(
				input_pixel_data,
				(std::uint8_t*)(&input_pixel_buffer),
				input_pixel_info.byte_count
			);

			std::uint8_t r_channel = 0;
			std::uint8_t g_channel = 0;
			std::uint8_t b_channel = 0;
			std::uint8_t a_channel = 0;

			// Pixel Decoding
			{
				std::uint8_t bit_index = 0;

				if (input_pixel_info.r_bits)
				{
					uint64_t r_bits_mask = (uint64_t)pow(2, input_pixel_info.r_bits) - 1;
					r_channel = static_cast<std::uint8_t>(r_bits_mask & input_pixel_buffer);
					
					if (input_pixel_info.r_bits < output_pixel_info.r_bits)
					{
						r_channel <<= output_pixel_info.r_bits - input_pixel_info.r_bits;
					}

					bit_index += input_pixel_info.r_bits;
				}

				if (input_pixel_info.g_bits)
				{
					std::uint64_t g_bits_mask = (std::uint64_t)pow(2, input_pixel_info.g_bits) - 1;
					g_channel = static_cast<std::uint8_t>(((g_bits_mask << bit_index) & input_pixel_buffer) >> bit_index);

					if (input_pixel_info.g_bits < output_pixel_info.g_bits)
					{
						g_channel <<= output_pixel_info.g_bits - input_pixel_info.g_bits;
					}
					
					bit_index += input_pixel_info.g_bits;
				}
				else
				{
					g_channel = r_channel;
				}

				if (input_pixel_info.b_bits)
				{
					std::uint64_t b_bits_mask = (std::uint64_t)pow(2, input_pixel_info.b_bits) - 1;
					b_channel = static_cast<std::uint8_t>(((b_bits_mask << bit_index) & input_pixel_buffer) >> bit_index);
					
					if (input_pixel_info.b_bits < output_pixel_info.b_bits)
					{
						b_channel <<= output_pixel_info.b_bits - input_pixel_info.b_bits;
					}

					bit_index += input_pixel_info.b_bits;
				}
				else
				{
					b_channel = g_channel;
				}

				if (input_pixel_info.a_bits)
				{
					std::uint64_t a_bits_mask = (std::uint64_t)pow(2, input_pixel_info.a_bits) - 1;
					a_channel = static_cast<std::uint8_t>(((a_bits_mask << bit_index) & input_pixel_buffer) >> bit_index);

					if (input_pixel_info.a_bits < output_pixel_info.a_bits)
					{
						a_channel <<= output_pixel_info.a_bits - input_pixel_info.a_bits;
					}
				}
				else
				{
					a_channel = 0xFF;
				}
			}

			// Some Pixel Type Specific actions
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

				std::vector<std::tuple<uint8_t, uint8_t>> channels = { 
					{output_pixel_info.r_bits, r_channel},
					{output_pixel_info.g_bits, g_channel},
					{output_pixel_info.b_bits, b_channel},
					{output_pixel_info.a_bits, a_channel},
				};

				for (auto& channel : channels)
				{
					auto& [bits, value] = channel;

					if (bits)
					{
						std::uint64_t bits_mask = (std::uint64_t)((pow(2, bits) - 1));
						output_pixel_buffer |= ((value & bits_mask) << bit_offset);

						bit_offset += bits;
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
}