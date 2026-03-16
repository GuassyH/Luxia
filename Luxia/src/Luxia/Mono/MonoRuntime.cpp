#include "lxpch.h"
#include "MonoRuntime.h"
#include <mono/metadata/object.h>

namespace Luxia::Mono {
	
    // Internal call target must have C linkage and a plain function pointer
    extern "C" static void Log_Line(MonoString* message) {
        char* msg = mono_string_to_utf8(message);
        LX_CORE_TRACE("[C#] {}", msg);
        mono_free(msg);
    }

    // Got help from OpenAI with an error here, with mono_jit. Relative paths wont work
	MonoRuntime::MonoRuntime() {
        // Get folder of the running executable
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        std::string exeDir = exePath;
        exeDir = exeDir.substr(0, exeDir.find_last_of("\\/"));

        // Compose paths to Mono lib and etc
        std::string monoLib = exeDir;
        std::string monoEtc = exeDir + "\\etc";

        mono_set_dirs(monoLib.c_str(), monoEtc.c_str());
        m_domain = mono_jit_init("Luxia");

        if (m_domain) {
            m_assembly = mono_domain_assembly_open(m_domain, (exeDir + std::string("/LuxiaBehaviour.dll")).c_str());
            if (m_assembly) {
				m_image = mono_assembly_get_image(m_assembly);
                if(m_image) {
                    // Register internal calls (define cs extern functions)
                    mono_add_internal_call("LuxiaBehaviour.Luxia.Log::Line", &Log_Line);
				
					MonoClass* testClass = mono_class_from_name(m_image, "LuxiaBehaviour", "LuxTest");
                    if (testClass) {
						// Describe the method signature: void TestMethod()
						MonoMethodDesc* m_MainDesc = mono_method_desc_new(".LuxTest:main()", false);
                        if (m_MainDesc) {
                            MonoMethod* m_MainMethod = mono_method_desc_search_in_class(m_MainDesc, testClass);
                            if(m_MainMethod){
								MonoObject* exception = nullptr;
								m_object = mono_runtime_invoke(m_MainMethod, nullptr, nullptr, &exception);
                                if (m_object) {
                                    LX_CORE_INFO("Successfully invoked C# method!");

									mono_gchandle_new(m_object, false); // Prevent GC from collecting the object

                                }

                                if (exception) {
                                    char* excMsg = mono_string_to_utf8(mono_object_to_string(exception, nullptr));
                                    LX_CORE_ERROR("Exception while invoking C# method: {}", excMsg);
                                    mono_free(excMsg);
                                }

                            }

							mono_method_desc_free(m_MainDesc);
                        }
                    }
                }
            }
        }
	}
	
	MonoRuntime::~MonoRuntime() {
		if (m_domain) {
			mono_jit_cleanup(m_domain);
			m_domain = nullptr;
		}
	}

}
