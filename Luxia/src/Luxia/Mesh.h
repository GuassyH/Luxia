#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/Vertex.h"
#include "Luxia/Rendering/API/IRenderBuffers.h"
#include "Luxia/Rendering/API/IMaterial.h"
#include "Luxia/Core/GUID.h"
#include "Luxia/Asset/Asset.h"

namespace Luxia {
	class LUXIA_API Mesh : public Assets::Asset {
	public:
		Mesh() { type = Luxia::AssetType::Mesh; }
		~Mesh() = default;

		uint16_t local_id = 0; // the id the model will use

		Mesh(std::vector<Rendering::Vertex> verts, std::vector<uint32_t> inds)
			: vertices(verts), indices(inds) {
			type = Luxia::AssetType::Mesh;
		}
		bool CalculateMesh();
			
		virtual bool Unload() override;

		const bool IsValid() const { return valid; }
		
		std::shared_ptr<Rendering::Buffers::VAO> vao = nullptr;
		std::shared_ptr<Rendering::Buffers::VBO> vbo = nullptr;
		std::shared_ptr<Rendering::Buffers::EBO> ebo = nullptr;

		std::vector<Rendering::Vertex> vertices;
		std::vector<uint32_t> indices;
		bool valid = false;
	private:
	};
}

