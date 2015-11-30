#pragma once

BEGINNAMESPACE

struct WindowDesc {
	const ansichar* title;
	union {
		int32 extents;
		struct {
			int32 width : 16;
			int32 height : 16;
		};
	};
	union {
		int32 position;
		struct {
			int32 posX : 16;
			int32 posY : 16;
		};
	};
	union {
		uint8 window_flags;
		struct {
			uint8 resizable : 1;
			uint8 visible : 1;
			uint8 decorated : 1;
			uint8 focused : 1;
			uint8 autoMinimize : 1;
			uint8 stereo : 1;
			uint8 floating : 1;
			uint8 taskbar : 1;
		};
	};

	union {
		uint16 opengl_flags;
		struct {
			uint8 doublebuffer : 1;
			uint8 debug_context : 1;
			uint8 forward_compat : 1;
			uint8 major_version : 5;
			uint8 minor_version : 8;
		};
	};

	union {
		uint32 buffer_bits;
		struct {
			uint32 red_bits : 5;
			uint32 green_bits : 5;
			uint32 blue_bits : 5;
			uint32 alpha_bits : 5;
			uint32 depth_bits : 5;
			uint32 stencil_bits : 5;
		};
	};

	union {
		uint32 accum_bits;
		struct {
			uint64 red_accum_bits : 5;
			uint64 green_accum_bits : 5;
			uint64 blue_accum_bits : 5;
			uint64 alpha_accum_bits : 5;
		};
	};

	uint32 aux_buffers;
	uint32 samples;
	uint32 refresh_rate;

	static const size_type MaxWindows = 10;
	static WindowDesc getDefault(); //> defined in Window.cpp
};

ENDNAMESPACE