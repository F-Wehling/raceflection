#pragma once

BEGINNAMESPACE

struct SourceInfo {
	inline SourceInfo(const ansichar* fi, const ansichar* fu, uint32 li) :
		file(fi), function(fu), line(li){}
	const ansichar *file;
	const ansichar *function;
	uint32 line;
};

#	define SOURCE_INFO SourceInfo(_FILE, _FUNCTION, _LINE)

ENDNAMESPACE