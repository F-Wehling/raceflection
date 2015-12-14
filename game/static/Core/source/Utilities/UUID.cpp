#include "Utilities/UUID.h"

#if IDE_MSVC
#	include <rpc.h>
#	pragma comment(lib, "rpcrt4.lib")
#else
//  THIS PACKAGE IS ONLY AVAILABE IF uuid-dev IS INSTALLED
//  HOPEFULLY IT IS INSTALLED IN LAB
#   include <uuid/uuid.h>
#endif

BEGINNAMESPACE

UUID generateUUID()
{
	UUID uuid;
#	if IDE_MSVC
	GUID guid;

	const size_type sof = sizeof(GUID);

	UuidCreate(&guid);

	uuid = *((UUID*)&guid);
#	else
    uuid_t guid;
    uuid_generate(guid);
    std::copy(guid, guid + sizeof(guid), uuid.data);
#	endif
	return uuid;
}

ENDNAMESPACE
