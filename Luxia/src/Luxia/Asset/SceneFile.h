#pragma once
#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API SceneFile : public AssetFile {
	public:
		SceneFile() = default;
		SceneFile(const std::filesystem::path m_scenePath)
			: scene_path(m_scenePath) {
			type = AssetType::SceneType;
		}
		~SceneFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		virtual bool Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			return true;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			return true;
		}

		std::filesystem::path scene_path;
	};
}
