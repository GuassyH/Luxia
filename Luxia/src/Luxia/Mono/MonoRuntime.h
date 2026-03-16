#pragma once

#include "Luxia/Core/Core.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

// Following tutorial from Lötwig Fusel
namespace Luxia::Mono {

	class LUXIA_API MonoRuntime {
	public:
		MonoRuntime();
		~MonoRuntime();

	private:
		MonoDomain* m_domain = nullptr;
		MonoAssembly* m_assembly = nullptr;
		MonoImage* m_image = nullptr;
		MonoObject* m_object = nullptr;
	};

};