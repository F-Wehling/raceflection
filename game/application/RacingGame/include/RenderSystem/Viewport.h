#pragma once

#include <Math/Math.h>

BEGINNAMESPACE

class Camera;
class RenderTarget;

class Viewport {

public:
	Camera* m_Camera;
	RenderTarget* m_RenderTarget;

	int32 m_ZOrder;
	Color m_BackgroundColor;
	float32 m_DepthClearValue;

};

ENDNAMESPACE