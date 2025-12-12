#include "lxpch.h"
#include "SceneSerializer.h"


namespace Luxia {
	SceneSerializer::SceneSerializer(std::shared_ptr<Luxia::Assets::SceneFile>& sceneFile) {
		m_SceneFile = sceneFile;
	}

	void SceneSerializer::SaveScene() {
		if (!m_SceneFile->loaded) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return; }

	} // save to file
	
	bool SceneSerializer::LoadScene() {
		if (!m_SceneFile->loaded) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return false; }

	} // load from file
}
