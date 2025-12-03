#pragma once

#include "Event.h"
#include "Luxia/Rendering/API/ITexture.h"

namespace Luxia {

	// Event for when the mouse moves
	class RenderCameraEvent : public Event {
	public:
		RenderCameraEvent(std::shared_ptr<ITexture> output, bool is_main, bool is_editor_cam = false)
			: output_texture(output), isMain(is_main), isEditorCam(is_editor_cam) {
		}

		std::shared_ptr<ITexture> GetTexture() { return output_texture; }
		bool IsMainCamera() const { return isMain; }
		bool IsEditorCamera() const { return isEditorCam; }

		GET_EVENT_TYPE(EventType::RenderCamera)
		GET_EVENT_CATEGORY(EventCategoryRender | EventCategoryWindow)


		std::string GetDebug() override {
			std::ostringstream stream; stream << "RenderCameraEvent: (" << output_texture->texID << ")";
			return stream.str();
		}

	private:
		bool isMain = false;
		bool isEditorCam = false;
		std::shared_ptr<ITexture> output_texture = nullptr;
	};
}