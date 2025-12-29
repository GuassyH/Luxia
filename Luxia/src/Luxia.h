#ifndef LUXIA_EXTERNAL_H
#define LUXIA_EXTERNAL_H

// APP SIDE ONLY
#include "Luxia/Core/Core.h"
#include "Luxia//Core/Log.h"
#include "Luxia/Core/Application.h"
#include "Luxia/Core/Time.h"

// Rendering
#include "Luxia/Rendering/API/IRenderer.h"
#include "Luxia/Rendering/API/IUIRenderer.h"

// Managers
#include "Luxia/Managers/EventManager.h"
#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Managers/SceneManager.h"
#include "Luxia/Managers/ProjectManager.h"

#include "Luxia/Scene/SceneSerializer.h"

// Events
#include "Luxia/Events/EventDefs.h"
#include "Luxia/Events/EventHandler.h"

// Components
#include "Luxia/Components/ComponentRegistry.h"

#include "Luxia/Entity.h"

// Rendering API
#include "Luxia/Platform/PlatformDefinitions.h"

// ---- Entry Point ----
// Now included in the actual main.cpp file of the application
// ---------------------

#endif