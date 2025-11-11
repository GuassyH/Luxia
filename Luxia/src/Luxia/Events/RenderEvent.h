#pragma once

#include "Event.h"
#include "Luxia/Rendering/API/ITexture.h"

namespace Luxia {

	// Event for when the mouse moves
	class RenderCameraEvent : public Event {
	public:
		RenderCameraEvent(std::shared_ptr<ITexture> output)
			: output_texture(output) {
		}

		std::shared_ptr<ITexture> GetTexture() { return output_texture; }
		
		GET_EVENT_TYPE(EventType::RenderCamera)
		GET_EVENT_CATEGORY(EventCategoryRender | EventCategoryWindow)


		std::string GetDebug() override {
			std::ostringstream stream; stream << "RenderCameraEvent: (" << output_texture->texID << ")";
			return stream.str();
		}

	private:
		std::shared_ptr<ITexture> output_texture;
	};
}