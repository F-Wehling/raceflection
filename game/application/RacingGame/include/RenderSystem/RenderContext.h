#pragma once

BEGINNAMESPACE

class RenderContext {
public:
	virtual ~RenderContext(){}

	virtual void swapBuffer() = 0;
	virtual void makeCurrent() = 0;
	virtual bool valid() = 0;
};

ENDNAMESPACE