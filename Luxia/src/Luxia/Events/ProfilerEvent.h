#pragma once

#include "Event.h"

namespace Luxia {

	class ProfilerRequestEvent : public Event {
	public:
		ProfilerRequestEvent(double time) : m_time(time) {}
		
		GET_EVENT_TYPE(EventType::RequestProfileData)
		GET_EVENT_CATEGORY(EventCategoryProfile)

		double GetTime() const { return m_time; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Profile Request Sent at time: (" << ")";
			return stream.str();
		}

	private:
		double m_time = 0.0;
	};

	class ProfilerResponseEvent : public Event {
	public:
		ProfilerResponseEvent(std::string name, double time) : m_name(name), m_time(time) {}

		GET_EVENT_TYPE(EventType::ProfileDataResponse)
		GET_EVENT_CATEGORY(EventCategoryProfile)

		double GetTime() const { return m_time; }
		std::string& GetName() { return m_name; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Profile Response : (" << m_name << ", " << m_time << ")";
			return stream.str();
		}

	private:
		std::string m_name = "Layer";
		double m_time = 0.0;
	};
}