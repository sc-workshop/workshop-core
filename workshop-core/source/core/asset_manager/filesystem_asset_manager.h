#pragma once

#include "asset_manager.h"

namespace wk {
	class FilesystemAssetManager : public AssetManager {
	public:
		virtual wk::Ref<wk::Stream> load_file(const std::filesystem::path&);
		virtual bool exists(const std::filesystem::path&);
	};
}