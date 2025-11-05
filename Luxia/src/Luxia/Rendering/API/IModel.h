#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Mesh.h"

#include "Luxia/Rendering/API/ITexture.h"

#include <filesystem>
#include <vector>
#include <memory>

namespace Luxia {
	class LUXIA_API IModel {
	public:

		IModel() = default;
		virtual ~IModel() = default;

		virtual void LoadFromFile(const std::filesystem::path& m_path) = 0;
		virtual void Unload() = 0;

		virtual void Cleanup() = 0;
		virtual void Delete() = 0;

		bool IsValid() const { return valid; }
		std::vector<Mesh>& GetMeshes() { return meshes; }

		std::string name = "no_name";
	protected:
		std::vector<Mesh> meshes;
		std::vector<std::shared_ptr<ITexture>> loaded_textures;
		std::filesystem::path path; // the name
		std::filesystem::path directory; // the way to the folder

		bool valid = false;
	};
}