#pragma once

namespace OIS {
	class Object;
	class Mouse;
}

BEGINNAMESPACE

class Mouse {
	friend class InputSystem;
public:
	typedef uint32 ButtonID;
	struct Button {
		static const ButtonID Left = 0;
		static const ButtonID Right = 1;
		static const ButtonID Middle = 2;
		static const ButtonID Button3 = 3;
		static const ButtonID Button4 = 4;
		static const ButtonID Button5 = 5;
		static const ButtonID Button6 = 6;
		static const ButtonID Button7 = 7;
	};
public:
	bool isPressed(ButtonID mouseButton);
	bool isReleased(ButtonID mouseButton);
	bool wentDown(ButtonID mouseButton);
	bool wentUp(ButtonID mouseButton);

	Point3f position() const;
	
protected:
	void capture();
	void setOISMouse(OIS::Object* keyboard);
    inline OIS::Mouse* getOISMouse() { return m_Mouse; }
	bool valid() const { return m_Mouse != nullptr; }
private:
	OIS::Mouse* m_Mouse;
	uint32 m_LastButtonState;
};

ENDNAMESPACE
