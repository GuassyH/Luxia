#pragma once

#include "Luxia/Asset/SceneFile.h"
#include "Luxia/Core/WeakPtrProxy.h"
#include "Luxia/Managers/AssetManager.h"

#include "Scene.h"
#include <yaml-cpp/yaml.h>


namespace Luxia {
	class LUXIA_API SceneSerializer
	{
	public:
		SceneSerializer(std::shared_ptr<Luxia::Assets::SceneFile> sceneFile, std::shared_ptr<Luxia::AssetManager> assetManager);

		void Serialize(); // save to file
		bool Deserialize(); // load from file
	private:
		WeakPtrProxy<Luxia::AssetManager> m_AssetManager;
		WeakPtrProxy<Luxia::Assets::SceneFile> m_SceneFile;
	};
}
