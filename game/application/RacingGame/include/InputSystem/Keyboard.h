#pragma once

namespace OIS {
	class Object;
	class Keyboard;
}

BEGINNAMESPACE

class Keyboard {
	friend class InputSystem;
public:
	typedef uint32 KeyCode;
	struct Code {
		static const KeyCode key_UNASSIGNED = 0x00;
		static const KeyCode key_ESCAPE = 0x01;
		static const KeyCode key_1 = 0x02;
		static const KeyCode key_2 = 0x03;
		static const KeyCode key_3 = 0x04;
		static const KeyCode key_4 = 0x05;
		static const KeyCode key_5 = 0x06;
		static const KeyCode key_6 = 0x07;
		static const KeyCode key_7 = 0x08;
		static const KeyCode key_8 = 0x09;
		static const KeyCode key_9 = 0x0A;
		static const KeyCode key_0 = 0x0B;
		static const KeyCode key_MINUS = 0x0C;    // - on main keyboard
		static const KeyCode key_EQUALS = 0x0D;
		static const KeyCode key_BACK = 0x0E;    // backspace
		static const KeyCode key_TAB = 0x0F;
		static const KeyCode key_Q = 0x10;
		static const KeyCode key_W = 0x11;
		static const KeyCode key_E = 0x12;
		static const KeyCode key_R = 0x13;
		static const KeyCode key_T = 0x14;
		static const KeyCode key_Y = 0x15;
		static const KeyCode key_U = 0x16;
		static const KeyCode key_I = 0x17;
		static const KeyCode key_O = 0x18;
		static const KeyCode key_P = 0x19;
		static const KeyCode key_LBRACKET = 0x1A;
		static const KeyCode key_RBRACKET = 0x1B;
		static const KeyCode key_RETURN = 0x1C;    // Enter on main keyboard
		static const KeyCode key_LCONTROL = 0x1D;
		static const KeyCode key_A = 0x1E;
		static const KeyCode key_S = 0x1F;
		static const KeyCode key_D = 0x20;
		static const KeyCode key_F = 0x21;
		static const KeyCode key_G = 0x22;
		static const KeyCode key_H = 0x23;
		static const KeyCode key_J = 0x24;
		static const KeyCode key_K = 0x25;
		static const KeyCode key_L = 0x26;
		static const KeyCode key_SEMICOLON = 0x27;
		static const KeyCode key_APOSTROPHE = 0x28;
		static const KeyCode key_GRAVE = 0x29;    // accent
		static const KeyCode key_LSHIFT = 0x2A;
		static const KeyCode key_BACKSLASH = 0x2B;
		static const KeyCode key_Z = 0x2C;
		static const KeyCode key_X = 0x2D;
		static const KeyCode key_C = 0x2E;
		static const KeyCode key_V = 0x2F;
		static const KeyCode key_B = 0x30;
		static const KeyCode key_N = 0x31;
		static const KeyCode key_M = 0x32;
		static const KeyCode key_COMMA = 0x33;
		static const KeyCode key_PERIOD = 0x34;    // . on main keyboard
		static const KeyCode key_SLASH = 0x35;    // / on main keyboard
		static const KeyCode key_RSHIFT = 0x36;
		static const KeyCode key_MULTIPLY = 0x37;    // * on numeric keypad
		static const KeyCode key_LMENU = 0x38;    // left Alt
		static const KeyCode key_SPACE = 0x39;
		static const KeyCode key_CAPITAL = 0x3A;
		static const KeyCode key_F1 = 0x3B;
		static const KeyCode key_F2 = 0x3C;
		static const KeyCode key_F3 = 0x3D;
		static const KeyCode key_F4 = 0x3E;
		static const KeyCode key_F5 = 0x3F;
		static const KeyCode key_F6 = 0x40;
		static const KeyCode key_F7 = 0x41;
		static const KeyCode key_F8 = 0x42;
		static const KeyCode key_F9 = 0x43;
		static const KeyCode key_F10 = 0x44;
		static const KeyCode key_NUMLOCK = 0x45;
		static const KeyCode key_SCROLL = 0x46;    // Scroll Lock
		static const KeyCode key_NUMPAD7 = 0x47;
		static const KeyCode key_NUMPAD8 = 0x48;
		static const KeyCode key_NUMPAD9 = 0x49;
		static const KeyCode key_SUBTRACT = 0x4A;    // - on numeric keypad
		static const KeyCode key_NUMPAD4 = 0x4B;
		static const KeyCode key_NUMPAD5 = 0x4C;
		static const KeyCode key_NUMPAD6 = 0x4D;
		static const KeyCode key_ADD = 0x4E;    // + on numeric keypad
		static const KeyCode key_NUMPAD1 = 0x4F;
		static const KeyCode key_NUMPAD2 = 0x50;
		static const KeyCode key_NUMPAD3 = 0x51;
		static const KeyCode key_NUMPAD0 = 0x52;
		static const KeyCode key_DECIMAL = 0x53;    // . on numeric keypad
		static const KeyCode key_OEM_102 = 0x56;    // < > | on UK/Germany keyboards
		static const KeyCode key_F11 = 0x57;
		static const KeyCode key_F12 = 0x58;
		static const KeyCode key_F13 = 0x64;    //                     (NEC PC98)
		static const KeyCode key_F14 = 0x65;    //                     (NEC PC98)
		static const KeyCode key_F15 = 0x66;    //                     (NEC PC98)
		static const KeyCode key_KANA = 0x70;    // (Japanese keyboard)
		static const KeyCode key_ABNT_C1 = 0x73;    // / ? on Portugese (Brazilian) keyboards
		static const KeyCode key_CONVERT = 0x79;    // (Japanese keyboard)
		static const KeyCode key_NOCONVERT = 0x7B;    // (Japanese keyboard)
		static const KeyCode key_YEN = 0x7D;    // (Japanese keyboard)
		static const KeyCode key_ABNT_C2 = 0x7E;    // Numpad . on Portugese (Brazilian) keyboards
		static const KeyCode key_NUMPADEQUALS = 0x8D;    //  = on numeric keypad (NEC PC98)
		static const KeyCode key_PREVTRACK = 0x90;    // Previous Track (static const KeyCode key_CIRCUMFLEX on Japanese keyboard)
		static const KeyCode key_AT = 0x91;    //                     (NEC PC98)
		static const KeyCode key_COLON = 0x92;    //                     (NEC PC98)
		static const KeyCode key_UNDERLINE = 0x93;    //                     (NEC PC98)
		static const KeyCode key_KANJI = 0x94;    // (Japanese keyboard)
		static const KeyCode key_STOP = 0x95;    //                     (NEC PC98)
		static const KeyCode key_AX = 0x96;    //                     (Japan AX)
		static const KeyCode key_UNLABELED = 0x97;    //                        (J3100)
		static const KeyCode key_NEXTTRACK = 0x99;    // Next Track
		static const KeyCode key_NUMPADENTER = 0x9C;    // Enter on numeric keypad
		static const KeyCode key_RCONTROL = 0x9D;
		static const KeyCode key_MUTE = 0xA0;    // Mute
		static const KeyCode key_CALCULATOR = 0xA1;    // Calculator
		static const KeyCode key_PLAYPAUSE = 0xA2;    // Play / Pause
		static const KeyCode key_MEDIASTOP = 0xA4;    // Media Stop
		static const KeyCode key_VOLUMEDOWN = 0xAE;    // Volume -
		static const KeyCode key_VOLUMEUP = 0xB0;    // Volume +
		static const KeyCode key_WEBHOME = 0xB2;    // Web home
		static const KeyCode key_NUMPADCOMMA = 0xB3;    // ; on numeric keypad (NEC PC98)
		static const KeyCode key_DIVIDE = 0xB5;    // / on numeric keypad
		static const KeyCode key_SYSRQ = 0xB7;
		static const KeyCode key_RMENU = 0xB8;    // right Alt
		static const KeyCode key_PAUSE = 0xC5;    // Pause
		static const KeyCode key_HOME = 0xC7;    // Home on arrow keypad
		static const KeyCode key_UP = 0xC8;    // UpArrow on arrow keypad
		static const KeyCode key_PGUP = 0xC9;    // PgUp on arrow keypad
		static const KeyCode key_LEFT = 0xCB;    // LeftArrow on arrow keypad
		static const KeyCode key_RIGHT = 0xCD;    // RightArrow on arrow keypad
		static const KeyCode key_END = 0xCF;    // End on arrow keypad
		static const KeyCode key_DOWN = 0xD0;    // DownArrow on arrow keypad
		static const KeyCode key_PGDOWN = 0xD1;    // PgDn on arrow keypad
		static const KeyCode key_INSERT = 0xD2;    // Insert on arrow keypad
		static const KeyCode key_DELETE = 0xD3;    // Delete on arrow keypad
		static const KeyCode key_LWIN = 0xDB;    // Left Windows key
		static const KeyCode key_RWIN = 0xDC;    // Right Windows key
		static const KeyCode key_APPS = 0xDD;    // AppMenu key
		static const KeyCode key_POWER = 0xDE;    // System Power
		static const KeyCode key_SLEEP = 0xDF;    // System Sleep
		static const KeyCode key_WAKE = 0xE3;    // System Wake
		static const KeyCode key_WEBSEARCH = 0xE5;    // Web Search
		static const KeyCode key_WEBFAVORITES = 0xE6;    // Web Favorites
		static const KeyCode key_WEBREFRESH = 0xE7;    // Web Refresh
		static const KeyCode key_WEBSTOP = 0xE8;    // Web Stop
		static const KeyCode key_WEBFORWARD = 0xE9;    // Web Forward
		static const KeyCode key_WEBBACK = 0xEA;    // Web Back
		static const KeyCode key_MYCOMPUTER = 0xEB;    // My Computer
		static const KeyCode key_MAIL = 0xEC;    // Mail
		static const KeyCode key_MEDIASELECT = 0xED;     // Media Select
	};
public:
	bool wentDown(KeyCode keyCode);
	bool wentUp(KeyCode keyCode);
	bool isPressed(KeyCode keyCode);
	bool isReleased(KeyCode keyCode);

protected:
	void capture();
	void setOISKeyboard(OIS::Object* keyboard);
    inline OIS::Keyboard* getOISKeyboard() { return m_KeyboardRef; }
	bool valid() const { return m_KeyboardRef != nullptr; }
private:
	OIS::Keyboard* m_KeyboardRef;
	Byte m_LastState[256];
};

ENDNAMESPACE
