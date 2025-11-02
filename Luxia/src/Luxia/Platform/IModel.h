#pragma once

#include "Luxia/Core.h"
#include <filesystem>
#include "Luxia/Log.h"

namespace Luxia {
	class LUXIA_API IModel {
	public:
		IModel() = default;
		virtual ~IModel() = default;

		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false) = 0;
		virtual void Unload() = 0;
		virtual void Render() = 0;

		virtual void Delete() = 0;

		bool IsValid() { return valid; }
	protected:
		std::string path;

		bool valid = false;
	};
}