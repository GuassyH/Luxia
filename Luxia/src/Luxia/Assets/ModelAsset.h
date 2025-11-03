#pragma once


#include "Luxia/Core.h"
#include "Asset.h"
#include <vector>
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
		virtual void Load(const std::filesystem::path& m_path) override {
			path = m_path;

			model = Luxia::Platform::Assets::CreateModel();
			
			bool suffix_found = false;
			if (path.has_extension()) {
				std::string m_suf = path.extension().string();

				for (auto& t_suf : p_suf) {
					if (t_suf == m_suf) {
						suffix = m_suf;
						suffix_found = true;
						break;
					}
				}

				std::string name_path = path.stem().string();
			}

			model->LoadFromFile(path);

			loaded = model->IsValid();

			if (loaded)
				LX_CORE_TRACE("Loaded Model Asset: '{}', {}", name, suffix);
			else
				LX_CORE_ERROR("Failed Loading Model Asset: {}", path.string());
		}
		virtual void Unload() override {
			model->Cleanup();
		}
		virtual void Delete() override {
			Unload();
			// Rest
		}
	private:
		std::array<std::string, 3> p_suf{
			".obj",
			".fbx",
			".gltf"
		};
	};
}