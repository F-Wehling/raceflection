#include "Utilities\UUID.h"

#if IDE_MSVC
#	include <rpc.h>
#	pragma comment(lib, "rpcrt4.lib")
#else
#	include <uuid/uuid.h>
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

#	endif
	return uuid;
}

ENDNAMESPACE
