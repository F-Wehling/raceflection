#pragma once

BEGINNAMESPACE

class RenderContext {
public:
	virtual ~RenderContext(){}

	inline void swapBuffer() { _swapBuffer(); }
	inline void makeCurrent() { _makeCurrent(); }
	inline bool valid() { return _valid(); }

protected:
	virtual void _swapBuffer() = 0;
	virtual void _makeCurrent() = 0;
	virtual bool _valid() = 0;
};

ENDNAMESPACE