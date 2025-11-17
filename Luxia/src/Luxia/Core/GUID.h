#pragma once

#include "Luxia/Core/Core.h"
#include <iostream>
#include <xhash>

namespace Luxia {
	class LUXIA_API GUID {
	public:
		GUID();
		GUID(uint64_t guid);
		GUID(const GUID&) = default;

		operator uint64_t() const { return m_guid; }
	private:
		uint64_t m_guid;
	};
}

namespace std {
	template <>
	struct hash<Luxia::GUID> {
		std::size_t operator()(const Luxia::GUID& guid) const {
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}