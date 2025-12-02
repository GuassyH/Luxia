#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Mesh.h"

#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Asset/ModelFile.h"

#include "Luxia/Asset/Asset.h"

#include <vector>
#include <filesystem>
#include <iostream>

namespace Luxia {
	class Scene;
	class LUXIA_API IModel : public Luxia::Assets::Asset {
	public:

		IModel() = default;
		virtual ~IModel() = default;

		virtual void LoadFromFile(const std::shared_ptr<Luxia::Assets::ModelFile> model_asset, Scene* active_scene) = 0; // temp shader
		virtual bool Unload() override = 0;

		bool IsValid() const { return valid; }
		std::vector<Mesh>& GetMeshes() { return meshes; }

		std::string name = "no_name";
	protected:
		std::vector<std::shared_ptr<ITexture>> loaded_textures; // to prevent duplicates
		std::vector<Mesh> meshes;
		std::filesystem::path path; // the name
		std::filesystem::path directory; // the way to the folder

		bool valid = false;
	};
}