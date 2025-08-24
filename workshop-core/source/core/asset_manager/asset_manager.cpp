#include "asset_manager.h"
#include "filesystem_asset_manager.h"

namespace wk {
	AssetManager& AssetManager::Instance() {
		if (m_instance == nullptr) {
			SetInstance(CreateRef<FilesystemAssetManager>());
		}

		return *m_instance;
	}

	void AssetManager::SetInstance(Ref<AssetManager> instance)
	{
		m_instance = instance;
	}
}