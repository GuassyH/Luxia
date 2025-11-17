#include "lxpch.h"
#include "GUID.h"

#include <random>

namespace Luxia {

	static std::random_device random_device;
	static std::mt19937_64 engine(random_device());
	static std::uniform_int_distribution<uint64_t> unfiorm_dist;

	GUID::GUID() : m_guid(unfiorm_dist(engine)) {
		
	}

	GUID::GUID(uint64_t guid) : m_guid(guid) {

	}
}