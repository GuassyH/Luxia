#include "lxpch.h"
#include "SceneSerializer.h"

#include "Luxia/Components/Camera.h"
#include "Luxia/CustomYAML.h"

namespace Luxia {

	SceneSerializer::SceneSerializer(std::shared_ptr<Luxia::Assets::SceneFile> sceneFile, std::shared_ptr<Luxia::AssetManager> assetManager) {
		m_SceneFile = sceneFile;
		m_AssetManager = assetManager;
	}

	static void SerializeEntity(Luxia::Entity& entity, YAML::Emitter& out) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.name;
		out << YAML::Key << "GUID" << YAML::Value << (uint64_t)entity.guid;
		out << YAML::Key << "Parent" << YAML::Value << (entity.transform->HasParent() ? (uint64_t)entity.transform->parent->ent_guid : 0);

		out << YAML::Key << "Components" << YAML::BeginMap;

		if (entity.transform) {
			out << YAML::Key << "Transform";

			out << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << entity.transform->position;
			out << YAML::Key << "EulerAngles" << YAML::Value << entity.transform->euler_angles;
			out << YAML::Key << "Scale" << YAML::Value << entity.transform->scale;

			out << YAML::EndMap;

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

				out << YAML::Key << "Mesh" << YAML::Value << (meshrend->mesh ? (uint64_t)meshrend->mesh->guid : 0);
				out << YAML::Key << "Material" << YAML::Value << (meshrend->material ? (uint64_t)meshrend->material->guid : 0);

				out << YAML::EndMap;
			}
		}

		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	// Save
	void SceneSerializer::Serialize() {
		if (!m_SceneFile) { LX_CORE_ERROR("Scene Serializer: scene file nullptr"); return; }
		if (!m_SceneFile->loaded || m_SceneFile->assets.empty()) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return; }


		std::shared_ptr<Scene> scene_raw = std::dynamic_pointer_cast<Scene>(m_SceneFile->assets[0]);
		if (!scene_raw) { return; }
		Scene& scene = *scene_raw;
		scene.scene_file = m_SceneFile;

		YAML::Emitter out;	

		out << YAML::BeginMap;
		out << YAML::Key << "AssetFileGUID" << YAML::Value << (uint64_t)m_SceneFile->guid;
		out << YAML::Key << "Name" << YAML::Value << scene.name;
		out << YAML::Key << "GUID" << YAML::Value << (uint64_t)scene.guid;
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
	
	static Luxia::Entity& DeserializeEntity(YAML::Node& entityNode, Luxia::Scene& scene, std::shared_ptr<Luxia::AssetManager> assetManager) {
		std::string name = entityNode["Entity"].as<std::string>();
		uint64_t guid = entityNode["GUID"].as<uint64_t>();

		Luxia::Entity& entity = scene.CreateEntity(name, guid);

		auto components = entityNode["Components"];
		if (!components)
			return entity;

		if (auto transNode = components["Transform"]) {
			entity.transform->position = transNode["Position"].as<glm::vec3>();
			entity.transform->euler_angles = transNode["EulerAngles"].as<glm::vec3>();
			entity.transform->scale = transNode["Scale"].as<glm::vec3>();

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

				uint64_t mesh_guid = 0;
				uint64_t mat_guid = 0;

				if(auto meshNode = meshRendNode["Mesh"])
					mesh_guid = meshNode.as<uint64_t>();
				if (auto matNode = meshRendNode["Material"])
					mat_guid = matNode.as<uint64_t>();

				// Mesh
				if (mesh_guid == 0) 
					meshRend.mesh = nullptr;
				else {
					if (assetManager->HasAsset<Luxia::Mesh>(mesh_guid)) 
						meshRend.mesh = assetManager->GetAsset<Luxia::Mesh>(mesh_guid);
					else 
						LX_CORE_ERROR("SceneSerializer: Deserializing Entity MeshRenderer failed, Mesh at GUID: {} - not found", (uint64_t)mesh_guid);
				}

				// Mat
				if (mat_guid == 0) 
					meshRend.material = nullptr;
				else {
					if (assetManager->HasAsset<Luxia::IMaterial>(mat_guid)) 
						meshRend.material = assetManager->GetAsset<Luxia::IMaterial>(mat_guid);
					else 
						LX_CORE_ERROR("SceneSerializer: Deserializing Entity MeshRenderer failed, Mat at GUID: {} - not found", (uint64_t)mat_guid);
				}

			}
		}


		return entity;
	}

	// Load
	bool SceneSerializer::Deserialize() {
		if (!m_SceneFile) { LX_CORE_ERROR("Scene Serializer: scene file nullptr"); return false; }
		if (!m_SceneFile->loaded || m_SceneFile->assets.empty()) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return false; }

		
		std::shared_ptr<Scene> scene_raw = std::dynamic_pointer_cast<Scene>(m_SceneFile->assets[0]);
		if (!scene_raw) { return false; }
		Scene& scene = *scene_raw;
		scene.scene_file = m_SceneFile;

		try {
			YAML::Node config = YAML::LoadFile(m_SceneFile->scene_path.string());
			scene.assetFileGUID = m_SceneFile->guid;

			// Initialize the entities
			auto entities = config["Entities"];
			if (!entities || !entities.IsSequence()) {
				return false;
			}

			for (auto entityNode : entities) {
				DeserializeEntity(entityNode, scene, m_AssetManager.lock());
			}
		
			// Set parents
			for (auto entityNode : entities) {
				Luxia::GUID entGUID = GUID(entityNode["GUID"].as<uint64_t>());
				if (scene.runtime_entities.contains(entGUID)) {
					Luxia::Entity& entity = scene.runtime_entities[entGUID];
					Luxia::GUID parentGUID = GUID(entityNode["Parent"].as<uint64_t>());
					if (scene.runtime_entities.contains(parentGUID)) {
						Luxia::Entity& parentEntity = scene.runtime_entities[parentGUID];
						entity.transform->SetParent(parentEntity.transform);
					}
				}
			}
		}
		catch (const YAML::Exception& ex) {
			std::cerr << "YAML error: " << ex.what() << "\n";
			return false;
		}

		return true;
	} // load from file
}
