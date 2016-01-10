#pragma once

BEGINNAMESPACE

struct CoordinateSystem {
	static const float32 POS_X[3]; // { 1.0, 0.0, 0.0 };
	static const float32 POS_Y[3]; // { 0.0, 1.0, 0.0 };
	static const float32 POS_Z[3]; // { 0.0, 0.0, 1.0 };
	static const float32 NEG_X[3]; // { -1.0, 0.0, 0.0 };
	static const float32 NEG_Y[3]; // { 0.0, -1.0, 0.0 };
	static const float32 NEG_Z[3]; // { 0.0, 0.0, -1.0 };

	static const float32* FORWARD;
	static const float32* UP;
	static const float32* RIGHT;
};

ENDNAMESPACE