#include "lxpch.h"
#include "SceneSerializer.h"

#include "Luxia/Components/Camera.h"
#include "Luxia/CustomYAML.h"

namespace Luxia {

	SceneSerializer::SceneSerializer(std::shared_ptr<Luxia::Assets::SceneFile> sceneFile) {
		m_SceneFile = sceneFile;
	}

	static void SerializeEntity(Luxia::Entity& entity, YAML::Emitter& out) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.name;
		out << YAML::Key << "GUID" << YAML::Value << (uint64_t)entity.guid;

		out << YAML::Key << "Components" << YAML::BeginMap;

		if (entity.transform) {
			out << YAML::Key << "Transform";

			out << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << entity.transform->position;
			out << YAML::Key << "EulerAngles" << YAML::Value << entity.transform->euler_angles;
			out << YAML::Key << "Scale" << YAML::Value << entity.transform->scale;

			out << YAML::EndMap;
		}

		auto cam = entity.transform->TryGetComponent<Luxia::Components::Camera>();
		if (cam) {
			out << YAML::Key << "Camera";
			out << YAML::BeginMap; // Camera

			out << YAML::Key << "FOV" << YAML::Value << cam->FOVdeg;
			out << YAML::Key << "FarPlane" << YAML::Value << cam->farPlane;
			out << YAML::Key << "NearPlane" << YAML::Value << cam->nearPlane;
			out << YAML::Key << "Main" << YAML::Value << cam->main;
			out << YAML::Key << "Width" << YAML::Value << cam->width;
			out << YAML::Key << "Height" << YAML::Value << cam->height;
			out << YAML::Key << "ClearColor" << YAML::Value << cam->clearColor;

			out << YAML::EndMap;
		}

		auto meshrend = entity.transform->TryGetComponent<Luxia::Components::MeshRenderer>();
		if (meshrend) {
			out << YAML::Key << "MeshRenderer";
			out << YAML::BeginMap; // Camera

			out << YAML::Key << "Material" << YAML::Value << (meshrend->material ? (uint64_t)meshrend->material->guid : 0);
			out << YAML::Key << "Mesh" << YAML::Value << (meshrend->mesh ? (uint64_t)meshrend->mesh->guid : 0);
			// out << YAML::Key << "Mesh" << YAML::Value << meshrend->mesh->guid;

			out << YAML::EndMap;
		}


		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	// Save
	void SceneSerializer::Serialize() {
		if (!m_SceneFile) { LX_CORE_ERROR("Scene Serializer: scene file nullptr"); return; }
		if (!m_SceneFile->loaded) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return; }


		std::shared_ptr<Scene> scene_raw = std::dynamic_pointer_cast<Scene>(m_SceneFile->assets[0]);
		if (!scene_raw) { return; }
		Scene& scene = *scene_raw;
		scene.scene_file = m_SceneFile;

		YAML::Emitter out;	

		out << YAML::BeginMap;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto& [guid, entity] : scene.runtime_entities) {
			SerializeEntity(entity, out);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;


		std::ofstream outfile(m_SceneFile->scene_path.string());
		outfile << out.c_str();
		outfile.close();


	} // save to file
	
	static Luxia::Entity& DeserializeEntity(YAML::Node& entityNode, Luxia::Scene& scene) {
		std::string name = entityNode["Entity"].as<std::string>();
		uint64_t guid = entityNode["GUID"].as<uint64_t>();

		Luxia::Entity& entity = scene.CreateEntity(name, guid);

		auto components = entityNode["Components"];
		if (!components)
			return entity;

		if (auto transNode = components["Transform"]) {
			LX_CORE_INFO("Serializing Transform");
			entity.transform->position = transNode["Position"].as<glm::vec3>();
			entity.transform->euler_angles = transNode["EulerAngles"].as<glm::vec3>();
			entity.transform->scale = transNode["Scale"].as<glm::vec3>();
		}

		if (auto camNode = components["Camera"]) {
			int width = camNode["Width"].as<int>();
			int height = camNode["Height"].as<int>();

			auto& cam = entity.transform->AddComponent<Luxia::Components::Camera>(width, height);
			cam.FOVdeg = camNode["FOV"].as<float>();
			cam.farPlane = camNode["FarPlane"].as<float>();
			cam.nearPlane = camNode["NearPlane"].as<float>();
			cam.main = camNode["Main"].as<bool>();
			cam.clearColor = camNode["ClearColor"].as<glm::vec4>();
		}

		if (auto meshRendNode = components["MeshRenderer"]) {
			auto& meshRend = entity.transform->AddComponent<Luxia::Components::MeshRenderer>();

			uint64_t mesh = 0;
			uint64_t mat = 0;

			if(auto meshNode = meshRendNode["Mesh"])
				mesh = meshNode.as<uint64_t>();
			if (auto matNode = meshRendNode["Material"])
				mat = matNode.as<uint64_t>();

			// Check asset/scene manager for mesh w guid and assign, etc
			if (mesh == 0)
				meshRend.mesh = nullptr;
			if (mat == 0)
				meshRend.material = nullptr;

		}

		return entity;
	}

	// Load
	bool SceneSerializer::Deserialize() {
		if (!m_SceneFile) { LX_CORE_ERROR("Scene Serializer: scene file nullptr"); return false; }
		if (!m_SceneFile->loaded) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return false; }
		
		std::shared_ptr<Scene> scene_raw = std::dynamic_pointer_cast<Scene>(m_SceneFile->assets[0]);
		if (!scene_raw) { return false; }
		Scene& scene = *scene_raw;
		scene.scene_file = m_SceneFile;

		try {
			YAML::Node config = YAML::LoadFile(m_SceneFile->scene_path.string());

			auto entities = config["Entities"];
			if (!entities || !entities.IsSequence()) {
				return false;
			}

			for (auto entityNode : entities) {
				DeserializeEntity(entityNode, scene);
			}
		
		}
		catch (const YAML::Exception& ex) {
			std::cerr << "YAML error: " << ex.what() << "\n";
			return false;
		}

		return true;
	} // load from file
}
