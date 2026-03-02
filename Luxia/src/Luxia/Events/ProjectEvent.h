#pragma once

#include "Event.h"

namespace Luxia {

	class OpenProjectEvent : public Event {
	public:
		OpenProjectEvent(std::string dir) : m_dir(dir) {}

		GET_EVENT_TYPE(EventType::OpenProject)
			GET_EVENT_CATEGORY(EventCategoryProject)

		std::string& GetDir() { return m_dir; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Project Open Event Dir (" << m_dir << ")";
			return stream.str();
		}

	private:
		std::string m_dir = "none";
	};

	class NewProjectEvent : public Event {
	public:
		NewProjectEvent(std::string dir, std::string name) : m_name(name), m_dir(dir) {}

		GET_EVENT_TYPE(EventType::NewProject)
		GET_EVENT_CATEGORY(EventCategoryProject)

		std::string& GetDir() { return m_dir; }
		std::string& GetName() { return m_name; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Project New Event Dir (" << m_dir << "\\" << m_name << ")";
			return stream.str();
		}

	private:
		std::string m_dir = "none";
		std::string m_name = "none";
	};
}