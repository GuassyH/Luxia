#include "lxpch.h"
#include "SceneSerializer.h"

#include "Luxia/Components/Camera.h"

namespace Luxia {
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(std::shared_ptr<Luxia::Assets::SceneFile>& sceneFile) {
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

		if (entity.transform->HasComponent<Luxia::Components::Camera>()) {
			auto cam = entity.transform->TryGetComponent<Luxia::Components::Camera>();

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


		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	// Save
	void SceneSerializer::Serialize(Luxia::Scene& scene) {
		if (!m_SceneFile->loaded) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return; }

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
	
	// Load
	bool SceneSerializer::Deserialize(Luxia::Scene& scene) {
		if (!m_SceneFile->loaded) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return false; }
		

		try {
			YAML::Node config = YAML::LoadFile(m_SceneFile->scene_path.string());

			// Check if missing
			// metallic = config["metallic"].as<float>();

		}
		catch (const YAML::Exception& ex) {
			std::cerr << "YAML error: " << ex.what() << "\n";
		}


	} // load from file
}
