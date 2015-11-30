#include <Utilities/PointerTools.h>

BEGINNAMESPACE

Byte* PointerTools::Align(Byte* byte, size_type alignment){
	Byte* ptr = byte;
	size_type off = (size_type)((UIntOfPtrSize)ptr & (alignment - 1));
	if (0 < off) off = alignment - off;	// number of bytes to skip
	
	ptr = (Byte*)ptr + off;
	return ptr;	
}

ENDNAMESPACE
