#pragma once

namespace OIS {
	class Object;
	class JoyStick;
};

BEGINNAMESPACE

class Joystick {
	friend class InputSystem;
	static const uint32 MaxAxisValue = 32768;
public:
	typedef uint32 ButtonID;
	struct Button {
		static const ButtonID _Offset = 0xFF00;
		static const ButtonID Button0 = 0x00;
		static const ButtonID Button1 = 0x01;
		static const ButtonID Button2 = 0x02;
		static const ButtonID Button3 = 0x03;
		static const ButtonID Button4 = 0x04;
		static const ButtonID Button5 = 0x05;
		static const ButtonID Button6 = 0x06;
		static const ButtonID Button7 = 0x07;
		static const ButtonID Button8 = 0x08;
		static const ButtonID Button9 = 0x09;
		static const ButtonID Button10 = 0xA;
		static const ButtonID Button11 = 0xB;
		static const ButtonID Button12 = 0xC;
		static const ButtonID Button13 = 0xD;
		static const ButtonID Button14 = 0xE;
		static const ButtonID Button15 = 0xF;
	};
	typedef uint32 PovID;
	struct POV{
		static const PovID Centered = 0x0000;
		static const PovID North = 0x0001;
		static const PovID South = 0x0010;
		static const PovID East  = 0x0100;
		static const PovID West  = 0x1000;
		static const PovID NorthEast = North | East; //=> 0x00000101;
		static const PovID SouthEast = South | East; //=> 0x00000110;
		static const PovID NorthWest = North | West; //=> 0x00001001;
		static const PovID SouthWest = South | West; //=> 0x00001010;
	};
public:
	bool isPressed(const ButtonID joystickButton);
	bool isReleased(const ButtonID joystickButton);
	bool wentDown(const ButtonID joystickButton);
	bool wentUp(const ButtonID joystickButton);

	PovID getPOV(uint32 povIdx);
	float32 getAxis(uint32 axisIdx);

protected:
	void capture();

	void setOISJoystick(OIS::Object* keyboard);
    OIS::JoyStick* getOISJoystick() {return m_Joystick;}
	bool valid() const { return m_Joystick != nullptr; }
private:
	OIS::JoyStick* m_Joystick; 
	uint32 m_LastButtonState;
};

ENDNAMESPACE
