#include "lxpch.h"
#include "SceneSerializer.h"

#include "Luxia/Components/Camera.h"

namespace YAML {
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

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

			out << YAML::Key << "Material" << YAML::Value << meshrend->material->guid;
			// out << YAML::Key << "Mesh" << YAML::Value << meshrend->mesh->guid;

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

		}

		return entity;
	}

	// Load
	bool SceneSerializer::Deserialize(Luxia::Scene& scene) {
		if (!m_SceneFile->loaded) { LX_CORE_ERROR("Scene Serializer: scene file not loaded"); return false; }
		

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
