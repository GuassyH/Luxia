#pragma once

#include "Luxia/Asset/SceneFile.h"
#include "Luxia/Core/WeakPtrProxy.h"

#include "Scene.h"
#include <yaml-cpp/yaml.h>


namespace Luxia {
	class LUXIA_API SceneSerializer
	{
	public:
		SceneSerializer(std::shared_ptr<Luxia::Assets::SceneFile> sceneFile);

		void Serialize(Luxia::Scene& scene); // save to file
		bool Deserialize(Luxia::Scene& scene); // load from file
	private:
		WeakPtrProxy<Luxia::Assets::SceneFile> m_SceneFile;
	};
}
