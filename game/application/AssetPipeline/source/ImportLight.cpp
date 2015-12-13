#include <ImportLight.h>

BEGINNAMESPACE

namespace Importer {
	Lights Importer::lightsAllFromScene(const aiScene * scene)
	{
		return Lights();
	}
	Lights lightsAllFromFile(const filesys::path & file)
	{
		return Lights();
	}
}

ENDNAMESPACE
