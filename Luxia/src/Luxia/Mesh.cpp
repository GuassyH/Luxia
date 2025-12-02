#include "lxpch.h"
#include "Mesh.h"
#include "Luxia/Rendering/RenderingCreation.h"

namespace Luxia {

	bool Mesh::CalculateMesh() {
		vao = Platform::Rendering::CreateVAO();
		vbo = Platform::Rendering::CreateVBO();
		ebo = Platform::Rendering::CreateEBO();

		if (!vao || !vbo || !ebo) { LX_CORE_ERROR("Mesh has invalid VAO, VBO, or EBO!"); return false; }

		vbo->BindBufferData(sizeof(Rendering::Vertex) * vertices.size(), vertices.data());

		// 0x1406 = GL_FLOAT, Temp
		vao->LinkAttrib(0, 3, 0x1406, sizeof(Rendering::Vertex), offsetof(Rendering::Vertex, pos));
		vao->LinkAttrib(1, 3, 0x1406, sizeof(Rendering::Vertex), offsetof(Rendering::Vertex, normal));
		vao->LinkAttrib(2, 2, 0x1406, sizeof(Rendering::Vertex), offsetof(Rendering::Vertex, texCoords));

		ebo->BindBufferData(indices.size() * sizeof(uint32_t), indices.data());

		vbo->Unbind();
		vao->Unbind();
		ebo->Unbind();

		valid = true;
		return true;
	}


	void Mesh::Cleanup() {
		vertices.clear(); 
		indices.clear(); 
		material = nullptr;
		valid = false; 
	}
}