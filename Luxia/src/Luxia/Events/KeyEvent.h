#pragma once

#include "Event.h"


namespace Luxia
{
	// Base class, defines that a KeyEvent takes a KeyCode and can return it
	class LUXIA_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		GET_EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}
		int m_KeyCode;
	};
	
	// Event for when key is pressed
	class LUXIA_API KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(int keycode)
			: KeyEvent(keycode) {}
		
		GET_EVENT_TYPE(EventType::KeyPressed)
		GET_EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

		std::string GetDebug() override {
			std::ostringstream ostr;
			ostr << "KeyPressEvent: " << m_KeyCode;
			return ostr.str();
		}
	private:
	};

	// Event for when key is released
	class LUXIA_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode)
			: KeyEvent(keycode) {
		}

		GET_EVENT_TYPE(EventType::KeyReleased)
		GET_EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

		std::string GetDebug() override {
			std::ostringstream ostr;
			ostr << "KeyPressEvent: " << m_KeyCode;
			return ostr.str();
		}
	};
}