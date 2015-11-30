#pragma once

#include <Utilities/Flags.h>
#include <vector>

BEGINNAMESPACE

class Resource {
public:
	DECLARE_MEM_EXCLUSIVE_FLAGS(Resource, Texture, Mesh, Light, Animation);

	virtual void write(OFileStream& file) = 0;

	ResourceFlags m_Type;
};

ENDNAMESPACE