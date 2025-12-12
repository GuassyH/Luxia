#pragma once

#include "Scene.h"
#include "Luxia/Asset/SceneFile.h"
#include "Luxia/Core/WeakPtrProxy.h"

namespace Luxia {
	class SceneSerializer
	{
	public:
		SceneSerializer(std::shared_ptr<Luxia::Assets::SceneFile>& sceneFile);

		void SaveScene(); // save to file
		bool LoadScene(); // load from file
	private:
		WeakPtrProxy<Luxia::Assets::SceneFile> m_SceneFile;
	};
}
