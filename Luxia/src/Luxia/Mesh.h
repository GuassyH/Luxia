#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/Vertex.h"
#include "Luxia/Rendering/API/IRenderBuffers.h"
#include "Luxia/Rendering/API/IMaterial.h"


namespace Luxia {
	class LUXIA_API Mesh {
	public:
		Mesh() = default;
		~Mesh() = default;

		Mesh(std::vector<Rendering::Vertex> verts, std::vector<uint32_t> inds, std::shared_ptr<IMaterial> mat = {})
			: vertices(verts), indices(inds), material(mat) {}
		bool CalculateMesh();

		void Cleanup();

		bool IsValid() const { return valid; }
		
		std::shared_ptr<Rendering::Buffers::VAO> vao = nullptr;
		std::shared_ptr<Rendering::Buffers::VBO> vbo = nullptr;
		std::shared_ptr<Rendering::Buffers::EBO> ebo = nullptr;

		std::vector<Rendering::Vertex> vertices;
		std::vector<uint32_t> indices;

		std::shared_ptr<Luxia::IMaterial> material = nullptr;

		std::string name = "no_name";
	private:
		bool valid = false;
	};
}

