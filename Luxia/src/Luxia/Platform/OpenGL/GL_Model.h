#pragma once

#include "Luxia/Rendering/API/IModel.h"


namespace Luxia::Platform {
	class GL_Model : public IModel {
	public:
		GL_Model() = default;
		~GL_Model() = default;

		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false) override {}
		virtual void Unload() override {}
		virtual void Render() override {}
	
		virtual void Cleanup() override {}
		virtual void Delete() override {}
	};
}