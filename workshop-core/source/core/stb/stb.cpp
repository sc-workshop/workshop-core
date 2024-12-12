#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#ifdef SC_MSVC

// Types cast warning
#pragma warning(disable:4244)

#endif

#include "core/stb/stb.h"

namespace wk
{
	namespace stb
	{
#pragma region Image Read
		int stbi_sc_io_read(void* user, char* data, int size)
		{
			Stream* stream = (Stream*)user;

			return static_cast<int>(stream->read(data, size));
		};

		void stbi_sc_io_skip(void* user, int n)
		{
			Stream* stream = (Stream*)user;

			stream->seek(n, Stream::SeekMode::Add);
		};

		int stbi_sc_io_eof(void* user)
		{
			Stream* stream = (Stream*)user;

			return stream->position() == stream->length() ? -1 : 0;
		};

		void load_image(Stream& stream, Ref<RawImage>& image_buffer)
		{
			int width, height, channels;

			// Image Info
			{
				size_t position = stream.position();
				int result = stbi_info_from_callbacks(&stbi_sc_io_callback, (void*)&stream, &width, &height, &channels);

				if (!result)
				{
					throw Exception("Stb loading exception! %s", stbi_failure_reason());
				}

				stream.seek(position);
			}

			Image::PixelDepth depth = Image::PixelDepth::RGB8;

			switch (channels)
			{
			case 1:
				depth = Image::PixelDepth::LUMINANCE8;
				break;

			case 2:
				depth = Image::PixelDepth::LUMINANCE8_ALPHA8;
				break;

			case 3:
				depth = Image::PixelDepth::RGB8;
				break;

			case 4:
				depth = Image::PixelDepth::RGBA8;
				break;

			default:
				break;
			}

			image_buffer = wk::CreateRef<RawImage>(
				static_cast<uint16_t>(width), static_cast<uint16_t>(height),
				depth, Image::ColorSpace::sRGB
			);

			// Image Loading
			{
				// memory usage ahhhhhh place
				uint8_t* data = stbi_load_from_callbacks(&stbi_sc_io_callback, (void*)&stream, &width, &height, &channels, 0);

				if (data == NULL)
				{
					throw Exception("Stb loading exception! %s", stbi_failure_reason());
				};

				Memory::copy(data, image_buffer->data(), image_buffer->data_length());
				stbi_image_free(data);
			}
		}
#pragma endregion

#pragma region Image Write
		void stbi_sc_io_write(void* user, void* data, int size)
		{
			Stream* stream = (Stream*)user;
			stream->write(data, size);
		}

		void write_image(RawImage& image, ImageFormat format, Stream& output)
		{
			uint8_t* buffer = nullptr;

			uint8_t channels = 0;

			Image::BasePixelType source_type = image.base_type();
			switch (source_type)
			{
			case Image::BasePixelType::RGBA:
				channels = 4;
				break;
			case Image::BasePixelType::RGB:
				channels = 3;
				break;
			case Image::BasePixelType::LA:
				channels = 2;
				break;
			case Image::BasePixelType::L:
				channels = 1;
				break;
			default:
				break;
			}

			Image::PixelDepth destination_depth = Image::PixelDepth::RGBA8;
			switch (source_type)
			{
			case Image::BasePixelType::RGBA:
				destination_depth = Image::PixelDepth::RGBA8;
				break;
			case Image::BasePixelType::RGB:
				destination_depth = Image::PixelDepth::RGB8;
				break;
			case Image::BasePixelType::LA:
				destination_depth = Image::PixelDepth::LUMINANCE8_ALPHA8;
				break;
			case Image::BasePixelType::L:
				destination_depth = Image::PixelDepth::LUMINANCE8;
				break;
			default:
				break;
			}

			if (image.depth() != destination_depth)
			{
				buffer = Memory::allocate(Image::calculate_image_length(image.width(), image.height(), destination_depth));

				Image::remap(
					image.data(), buffer,
					image.width(), image.height(),
					image.depth(), destination_depth
				);
			}

			int result = 0;
			switch (format)
			{
			case ImageFormat::PNG:
				result = stbi_write_png_to_func(&stbi_sc_io_write, (void*)&output, image.width(), image.height(), channels, buffer ? buffer : image.data(), 0);
				break;
			case stb::ImageFormat::BMP:
				result = stbi_write_bmp_to_func(&stbi_sc_io_write, (void*)&output, image.width(), image.height(), channels, buffer ? buffer : image.data());
				break;
			case stb::ImageFormat::TGA:
				result = stbi_write_tga_to_func(&stbi_sc_io_write, (void*)&output, image.width(), image.height(), channels, buffer ? buffer : image.data());
				break;
			case stb::ImageFormat::JPEG:
				result = stbi_write_jpg_to_func(&stbi_sc_io_write, (void*)&output, image.width(), image.height(), channels, buffer ? buffer : image.data());
				break;
			default:
				break;
			}

			if (buffer)
			{
				free(buffer);
			}

			if (result == 0)
			{
				throw Exception("Stb writing exception!");
			}
		}

		void write_image(RawImage& image, std::string extension, Stream& output)
		{
			ImageFormat format = ImageFormat::PNG;

			if (extension.empty())
			{
				extension = ".png";
			}

			if (extension.at(0) == '.')
			{
				extension = std::string(extension.begin() + 1, extension.end());
			}

			if (extension == "png")
			{
				format = ImageFormat::PNG;
			}

			if (extension == "jpg" || extension == "jpeg")
			{
				format = ImageFormat::JPEG;
			}

			if (extension == "tga")
			{
				format = ImageFormat::TGA;
			}

			if (extension == "bmp")
			{
				format = ImageFormat::BMP;
			}

			write_image(image, format, output);
		}

#pragma endregion
	}
}