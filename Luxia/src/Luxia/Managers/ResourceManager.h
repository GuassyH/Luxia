#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Mesh.h"

namespace Luxia {
	class AssetManager;

	class LUXIA_API ResourceManager {
	public:
		static void Init();
		static void Cleanup();

		// Meshes
		static std::shared_ptr<Luxia::Mesh> DefaultSphere;
		static std::shared_ptr<Luxia::Mesh> DefaultCube;
		static std::shared_ptr<Luxia::Mesh> DefaultPlane;



		// Materials
		static std::shared_ptr<Luxia::IShader> NullShader;
		static std::shared_ptr<Luxia::IMaterial> NullMaterial;

		static std::shared_ptr<Luxia::IShader> DefaultUnlitShader;
		static std::shared_ptr<Luxia::IMaterial> DefaultUnlitMaterial;

		static std::shared_ptr<Luxia::IShader> DefaultLitShader;
		static std::shared_ptr<Luxia::IMaterial> DefaultLitMaterial;

	};
}

