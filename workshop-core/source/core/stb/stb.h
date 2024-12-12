#pragma once

#include "core/exception/exception.h"

#include "core/io/stream.h"
#include "core/memory/memory.h"
#include "core/image/raw_image.h"
#include "core/preprocessor/api.h"
#include "core/memory/ref.h"

#pragma region STB Image Defines
#define STBIR_MALLOC(size, c) ((void)(c), wk::Memory::allocate(size))
#define STBIR_FREE(ptr,c)    ((void)(c), free(ptr))

#include "core/3rdparty/stb/stb_image_resize.h"
#pragma endregion STB Image Resize Defines

#pragma region
#define STBI_MALLOC(sz)           wk::Memory::allocate(sz)
#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
#define STBI_FREE(p)              free(p)

#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PNM
#define STBI_NO_PIC
#define STBI_NO_STDIO

#include "core/3rdparty/stb/stb_image.h"
#pragma endregion

#pragma region STB Image Write Defines
#define STBIW_MALLOC(sz)        wk::Memory::allocate(sz)
#define STBIW_REALLOC(p,newsz)  realloc(p,newsz)
#define STBIW_FREE(p)           free(p)

#define STBI_WRITE_NO_STDIO

#include "core/3rdparty/stb/stb_image_write.h"
#pragma endregion

namespace wk
{
	namespace stb
	{
#pragma region Image Read
		int WORKSHOP_API stbi_sc_io_read(void* user, char* data, int size);
		void WORKSHOP_API stbi_sc_io_skip(void* user, int n);
		int WORKSHOP_API stbi_sc_io_eof(void* user);

		const stbi_io_callbacks stbi_sc_io_callback = { &stbi_sc_io_read, &stbi_sc_io_skip, &stbi_sc_io_eof };

		/// <summary>
		/// Loads png, jpeg, psd, tga and bmp images
		/// </summary>
		/// <param name="stream"></param>
		/// <returns>Loaded image</returns>
		void WORKSHOP_API load_image(Stream& stream, Ref<RawImage>& image);
#pragma endregion

		enum class ImageFormat : uint8_t
		{
			PNG = 0,
			BMP,
			TGA,
			JPEG
		};

#pragma region Image Write
		void WORKSHOP_API stbi_sc_io_write(void* user, void* data, int size);

		void WORKSHOP_API write_image(RawImage& image, ImageFormat format, Stream& output);

		/// <summary>
		/// Saves images depending on path extension
		/// </summary>
		/// <param name="image"></param>
		/// <param name="extension"> Must be in lowercase </param>
		/// <param name="output"></param>
		void WORKSHOP_API write_image(RawImage& image, std::string extension, Stream& output);
#pragma endregion
	}
}