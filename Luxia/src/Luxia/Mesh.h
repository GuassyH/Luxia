#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/Vertex.h"
#include "Luxia/Rendering/API/IRenderBuffers.h"
#include <vector>
#include <iostream>
#include "Luxia/Rendering/API/ITexture.h"

namespace Luxia {
	class LUXIA_API Mesh {
	public:
		Mesh() = default;
		~Mesh() = default;

		Mesh(std::vector<Rendering::Vertex> verts, std::vector<uint32_t> inds, std::vector<std::shared_ptr<ITexture>> texs) 
			: vertices(verts), indices(inds), textures(texs) {}
		bool CalculateMesh();

		void Cleanup();

		bool IsValid() const { return valid; }
		
		std::shared_ptr<Rendering::Buffers::VAO> vao = nullptr;
		std::shared_ptr<Rendering::Buffers::VBO> vbo = nullptr;
		std::shared_ptr<Rendering::Buffers::EBO> ebo = nullptr;

		std::vector<Rendering::Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<ITexture>> textures;

		std::string name = "no_name";
	private:
		bool valid = false;
	};
}

