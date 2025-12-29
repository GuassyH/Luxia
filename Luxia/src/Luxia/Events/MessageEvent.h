#pragma once

#include "Event.h"

namespace Luxia {

	// Event for when the mouse moves
	class MessageSentEvent : public Event {
	public:
		MessageSentEvent(std::string& message)
			: m_message(message) {
		}		
		MessageSentEvent(const char* message)
			: m_message(std::string(message)) {
		}

		GET_EVENT_TYPE(EventType::MessageSent)
		GET_EVENT_CATEGORY(EventCategoryMessage)

		std::string& GetMessageStr() { return m_message; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Message Sent: (" << m_message << ")";
			return stream.str();
		}

	private:
		std::string m_message;
	};
}