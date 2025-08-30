#pragma once

#include <filesystem>

#include "core/memory/ref.h"
#include "core/io/stream.h"

namespace wk {
	class AssetManager {
	public:
		static AssetManager& Instance();
		virtual ~AssetManager() = default;

	public:
		static void SetInstance(Ref<AssetManager> instance);

	private:
		static thread_local inline Ref<AssetManager> m_instance = nullptr;

	public:
		virtual wk::Ref<wk::Stream> load_file(const std::filesystem::path&) = 0;
		virtual wk::Ref<wk::Stream> write_file(const std::filesystem::path&) = 0;
		virtual bool exists(const std::filesystem::path&) = 0;
	};
}