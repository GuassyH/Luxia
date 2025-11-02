#pragma once

#include "IModel.h"


namespace Luxia::Platform {
	class Win_Model : public IModel {
	public:
		Win_Model() = default;
		~Win_Model() = default;

		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false) override {}
		virtual void Unload() override {}
		virtual void Render() override {}

		virtual void Delete() override {}
	};
}