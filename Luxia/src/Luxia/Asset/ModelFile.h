#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Platform/PlatformDefinitions.h"
#include "Luxia/Mesh.h"

namespace Luxia::Assets {
	class LUXIA_API ModelFile : public AssetFile {
	public:
		ModelFile() = default;
		ModelFile(const std::filesystem::path m_modelPath)
			: modelPath(m_modelPath) {
			type = AssetType::ModelType;
		}
		ModelFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~ModelFile() = default;

		std::filesystem::path modelPath;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		void SerializeMesh(YAML::Emitter& out, std::shared_ptr<Asset> mesh) {

		}

		void DeserializeMesh(YAML::Node& node) {
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		}

		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			loaded = false;

			std::vector<std::shared_ptr<Asset>> meshes = std::vector<std::shared_ptr<Asset>>(0);


			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				// Check if missing
				modelPath = config["model_path"].as<std::string>();
				auto meshNodes = config["Meshes"];
				if (!meshNodes || !meshNodes.IsSequence()) {
					assets = meshes;
					return assets;
				}

				for (auto meshNode : meshNodes) {
					DeserializeMesh(meshNode);
				}

				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				loaded = false;
			}

			assets = meshes;

			return assets;
		}


		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			YAML::Emitter out;

			out << YAML::BeginMap;

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
