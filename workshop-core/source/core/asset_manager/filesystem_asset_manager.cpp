#include "filesystem_asset_manager.h"

#include "core/io/file_stream.h"
#include "core/exception/exception.h"

namespace wk {
	Ref<Stream> FilesystemAssetManager::load_file(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
			throw wk::Exception("Failed to load asset: " + path.filename().string());

		return wk::CreateRef<InputFileStream>(path);
	}

	wk::Ref<wk::Stream> FilesystemAssetManager::write_file(const std::filesystem::path& path)
	{
		return wk::CreateRef<OutputFileStream>(path);
	}

	bool FilesystemAssetManager::exists(const std::filesystem::path& path) {
		return std::filesystem::exists(path);
	}
}
