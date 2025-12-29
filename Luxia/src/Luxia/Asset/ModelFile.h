#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Platform/PlatformDefinitions.h"
#include "Luxia/Mesh.h"

namespace Luxia::Assets {
	class LUXIA_API ModelFile : public AssetFile {
	public:
		ModelFile() = default;
		ModelFile(const std::filesystem::path m_modelPath)
			: modelPath(m_modelPath.lexically_normal()) {}
		ModelFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~ModelFile() = default;

		std::filesystem::path modelPath;
		std::shared_ptr<Luxia::IModel> base_model;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::ModelType;
			guid = GUID();

			base_model = Platform::Assets::CreateModel();
			base_model->assetFileGUID = guid;
			base_model->guid = GUID();

			std::vector<std::shared_ptr<Mesh>> meshes = base_model->LoadFromPath(modelPath);

			for (auto& mesh : meshes) {
				mesh->assetFileGUID = guid;
				mesh->guid = GUID();
				assets.push_back(mesh);
			}

			Save(m_assetPath);
			return true;
		}

		void SerializeMesh(YAML::Emitter& out, std::shared_ptr<Mesh> mesh) {
			out << YAML::BeginMap;
			out << YAML::Key << "Mesh" << YAML::Value << mesh->name;
			out << YAML::Key << "GUID" << YAML::Value << (uint64_t)mesh->guid;
			out << YAML::Key << "LocalID" << YAML::Value << (uint64_t)mesh->local_id;
			out << YAML::EndMap;
		}

		std::shared_ptr<Mesh> DeserializeMesh(YAML::Node& meshNode, std::shared_ptr<IModel> model) {
			// get the data from the mesh node
			std::string name = meshNode["Mesh"].as<std::string>();
			uint64_t guid = meshNode["GUID"].as<uint64_t>();
			uint16_t lid = meshNode["LocalID"].as<uint16_t>();

			// Get the mesh from the model, assumes same model saved and loaded
			std::shared_ptr<Mesh>& mesh = model->meshes[lid];

			if (!mesh)
				return nullptr;

			mesh->name = name;
			mesh->guid = guid;
			mesh->assetFileGUID = guid;
			mesh->local_id = lid; // useless but why not

			return mesh;
		}

		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::ModelType;

			assetPath = m_assetPath.lexically_normal();
			loaded = false;

			base_model = Platform::Assets::CreateModel();

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				guid = GUID(config["AssetFileGUID"].as<uint64_t>());

				modelPath = config["ModelPath"].as<std::string>();
				std::vector<std::shared_ptr<Mesh>> meshes = base_model->LoadFromPath(modelPath);

				base_model->name = config["Model"].as<std::string>();
				base_model->guid = config["GUID"].as<uint64_t>();
				base_model->assetFileGUID = guid;

				auto meshNodes = config["Meshes"];
				if (!meshNodes || !meshNodes.IsSequence()) {
					return assets;
				}

				// For each meshNode, Deserialize
				for (auto meshNode : meshNodes) {
					auto mesh = DeserializeMesh(meshNode, base_model);
					if (mesh) {
						assets.push_back(mesh);
					}
				}

				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				loaded = false;
			}

			return assets;
		}


		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			if (assets.empty()) return false;
			if (!base_model) return false;
			// Only save if the assetfile path already exists??

			YAML::Emitter out;

			out << YAML::BeginMap;

			out << YAML::Key << "AssetFileGUID" << YAML::Value << (uint64_t)guid;
			out << YAML::Key << "ModelPath" << YAML::Value << modelPath.string();
			out << YAML::Key << "Model" << YAML::Value << base_model->name;
			out << YAML::Key << "GUID" << YAML::Value << (uint64_t)base_model->guid;
			out << YAML::Key << "Meshes" << YAML::Value << YAML::BeginSeq;

			for (auto& meshptr : assets) {
				if (meshptr) {
					std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(meshptr);
					if (mesh) {
						SerializeMesh(out, mesh);
					}
				}
			}

			out << YAML::EndSeq;

			out << YAML::EndMap;

			if (!out.good()) {
				LX_CORE_ERROR("Emitter failed!");
				return false;
			}


			// Write to file
			std::ofstream outfile(assetPath); // text mode
			if (!outfile.is_open()) {
				LX_CORE_ERROR("Failed to save metafile: {}", assetPath.string());
				return false;
			}

			outfile << out.c_str(); // write YAML text

			outfile.close();

			return true;
		}

	};
}
