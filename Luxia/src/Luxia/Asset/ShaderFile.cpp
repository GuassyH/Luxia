#include "lxpch.h"

#include "ShaderFile.h"

namespace Luxia::Assets {

	bool ShaderFile::LoadExtra(std::ifstream& m_infile) {
		int success = 0;
		std::string line;
		while (std::getline(m_infile, line)) {
			// Trim whitespace if needed
			if (line.rfind("frag=", 0) == 0) {
				fragPath = line.substr(5);
				success++;
			}
			else if (line.rfind("vert=", 0) == 0) {
				vertPath = line.substr(5);
				success++;
			}
		}

		if (success != 2) { return false; }

		return true;
	}
	bool ShaderFile::SaveExtra(std::ofstream& m_outfile) {

		m_outfile << "Shader:\n";
		m_outfile << "frag=" << fragPath.string() << "\n";
		m_outfile << "vert=" << vertPath.string() << "\n";
		LX_CORE_TRACE("frag {}\nvert {}", fragPath.string(), vertPath.string());

		return true;
	}
}