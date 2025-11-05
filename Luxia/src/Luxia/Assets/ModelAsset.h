#pragma once
#include "Asset.h"

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Platform/AssetCreation.h"

using Luxia::IModel;

namespace Luxia::Assets {

	class LUXIA_API ModelAsset : public Asset {
	public:
		std::shared_ptr<Luxia::IModel> model = nullptr;

		ModelAsset() {
			type = AssetType::Model;
			loaded = false;
		}


		virtual void Create(const std::filesystem::path& sourcePath) override {
			srcPath = sourcePath;

			model = Luxia::Platform::Assets::CreateModel();
			
			bool suffix_found = false;
			if (srcPath.has_extension()) {
				std::string m_suf = srcPath.extension().string();

				for (auto& t_suf : p_suf) {
					if (t_suf == m_suf) {
						suffix = m_suf;
						suffix_found = true;
						break;
					}
				}
			}

			model->LoadFromFile(srcPath);
			name = model->name;

			loaded = model->IsValid();

			if (loaded)
				LX_CORE_TRACE("Created Model Asset: '{}', {}", name, suffix);
			else
				LX_CORE_ERROR("Failed Created Model Asset: {}", srcPath.string());
		}

		virtual void Load(const std::filesystem::path& metaFile) {

		}


		virtual void Unload() override {
			model->Cleanup();
		}
	private:
		std::array<std::string, 3> p_suf{
			".obj",
			".fbx",
			".gltf"
		};
	};
}