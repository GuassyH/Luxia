#pragma once

#include "Luxia/Core.h"
#include "Luxia/Rendering/Vertex.h"
#include "Luxia/Rendering/API/IRenderBuffers.h"
#include <vector>
#include <iostream>

namespace Luxia {
	class LUXIA_API Mesh {
	public:
		Mesh() = default;
		~Mesh() = default;

		Mesh(std::vector<Rendering::Vertex> verts, std::vector<uint32_t> inds) : vertices(verts), indices(inds) {}
		bool CalculateMesh();

		void Cleanup() { vertices.clear(); indices.clear(); valid = false; }

		bool IsValid() const { return valid; }
		
		std::shared_ptr<Rendering::Buffers::VAO> vao = nullptr;
		std::shared_ptr<Rendering::Buffers::VBO> vbo = nullptr;
		std::shared_ptr<Rendering::Buffers::EBO> ebo = nullptr;

		std::vector<Rendering::Vertex> vertices;
		std::vector<uint32_t> indices;
	private:
		bool valid = false;
	};
}

