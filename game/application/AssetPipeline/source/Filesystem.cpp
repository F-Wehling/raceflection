#include "Filesystem.h"

#if OS_WINDOWS
#	include <Windows.h>
#endif

BEGINNAMESPACE

filesys::path filesys::extension(const path & p)
{
	size_type n = p.find_last_of('.');
	if (n == path::npos) return path();
	return p.substr(n);
}

filesys::path filesys::concat(const path & p1, const path & p2)
{
	if (p1.back() == '/') {
		return p1 + p2;
	}
	return p1 + '/' + p2;
}

size_type filesys::file_size(const path & p)
{
	IFileStream f(p.c_str(), std::ios::in | std::ios::ate);
	if (!f.is_open())  return 0;
	size_type s = f.tellg();
	f.close();
	return s;
}

bool filesys::is_directory(const path & p)
{
#	if OS_WINDOWS
	DWORD fileAttrib = GetFileAttributes(p.c_str());
	if (fileAttrib == INVALID_FILE_ATTRIBUTES) return false;
	return (fileAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
#	else
	return false;
#	endif
}

bool filesys::is_regular_file(const path & p)
{
#	if OS_WINDOWS
	DWORD fileAttrib = GetFileAttributes(p.c_str());
	if (fileAttrib == INVALID_FILE_ATTRIBUTES) return false;
	return (fileAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0; //For now: not a directory -> file
#	else
	return false;
#	endif
}

#if OS_WINDOWS
time_t filetime_to_timet(FILETIME const& ft)
{
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	return ull.QuadPart / 10000000ULL - 11644473600ULL;
}
#endif

uint32 filesys::last_write_time(const path & p)
{
#	if OS_WINDOWS
	FILETIME ftCreate, ftAccess, ftLastWrite;
	HANDLE hFile = CreateFile(p.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	if (0 != GetFileTime(hFile, &ftCreate, &ftAccess, &ftLastWrite)) return 0;
	return filetime_to_timet(ftLastWrite);
#	else

#	endif
	return uint32();
}

filesys::path filesys::stem(const path & p)
{
	size_type n = 0, m = p.length() - extension(p).length();
	
	if (p.find_last_of('/') != path::npos) {
		n = p.find_last_of('/') + 1;
	}
	return p.substr(n, m - n);
}

bool filesys::create_directory(const path & p)
{
#	if OS_WINDOWS
	return CreateDirectory(p.c_str(), 0) != 0;
#	else
#	endif
}

bool filesys::exists(const path & p)
{
#	if OS_WINDOWS
	return file_size(p) != 0;
#	else
	return false;
#	endif
}

filesys::DirectoryIterator::DirectoryIterator() :
	hdl(nullptr), current("")
{
}

filesys::DirectoryIterator::DirectoryIterator(const path & p, const path& filter /* = "*.*"*/)
{
#	if OS_WINDOWS
	WIN32_FIND_DATA ffd;
	path p2 = filesys::concat(p, filter);
	basePath = p;
	hdl = FindFirstFile(p2.c_str(), &ffd);
	if (hdl == INVALID_HANDLE_VALUE) 
		hdl = nullptr;
	current = filesys::concat(basePath,path(ffd.cFileName));
	if (path(ffd.cFileName) == "." || path(ffd.cFileName) == "..")
		operator ++();
#	else

#	endif
}

filesys::DirectoryIterator & filesys::DirectoryIterator::operator++()
{
#	if OS_WINDOWS
	if (hdl == nullptr) return *this;
	WIN32_FIND_DATA ffd;

	while (true) {
		if (FindNextFile(hdl, &ffd) == 0) {
			hdl = nullptr;
			current = path();
			return *this;
		}
		if (path(ffd.cFileName) != "." && path(ffd.cFileName) != "..")
			break;
	}
	
	current = filesys::concat(basePath,path(ffd.cFileName));
	return *this;
#	else

#	endif
}

const filesys::path & filesys::DirectoryIterator::operator*() const
{
	return current;
}

filesys::DirectoryIterator::operator bool() const
{
#	if OS_WINDOWS
	return hdl != nullptr;
#	else

#	endif
}

bool filesys::DirectoryIterator::operator==(const DirectoryIterator & rhs) const
{
#	if OS_WINDOWS
	return hdl == rhs.hdl;
#	else
#	endif
}

ENDNAMESPACE
