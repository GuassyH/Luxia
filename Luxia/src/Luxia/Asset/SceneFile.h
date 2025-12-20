#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Scene/Scene.h"

namespace Luxia::Assets {
	class LUXIA_API SceneFile : public AssetFile {
	public:
		SceneFile() = default;
		SceneFile(const std::filesystem::path m_scenePath)
			: scene_path(m_scenePath) {
			type = Luxia::AssetType::SceneType;
		}
		SceneFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~SceneFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::SceneType;

			assetPath = m_assetPath;
			scene_path = assetPath;

			std::shared_ptr<Asset> scene = std::make_shared<Scene>();
			scene->name = m_assetPath.filename().replace_extension("").string();
			scene->guid = GUID();

			LX_CORE_INFO("Scene Files are not loaded regularly");

			assets.push_back(scene);
			loaded = true;

			Save(m_assetPath);
			return true;
		}

		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::SceneType;

			assetPath = m_assetPath;
			scene_path = assetPath;

			std::shared_ptr<Asset> scene = std::make_shared<Scene>();

			LX_CORE_INFO("Scene Files are not loaded regularly");

			assets.push_back(scene);
			loaded = true;

			return assets;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			scene_path = assetPath;
			
			LX_CORE_INFO("Scene Files are not saved regularly");

			return true;
		}

		std::filesystem::path scene_path;
	};
}
