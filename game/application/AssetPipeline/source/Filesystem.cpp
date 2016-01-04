#include "Filesystem.h"

#if OS_WINDOWS
#	include <Windows.h>
#	include <sys/stat.h>
#	include <dirent.h>
#   include <io.h>
#elif OS_LINUX
#   include <sys/stat.h>
#   include <dirent.h>
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
	struct stat stats;
	if (stat(p.c_str(), &stats) != 0) return 0;
	return stats.st_size;
}

bool filesys::is_directory(const path & p)
{
	/*
	DWORD fileAttrib = GetFileAttributes(p.c_str());
	if (fileAttrib == INVALID_FILE_ATTRIBUTES) return false;
	return (fileAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0;	
	*/
#	if OS_WINDOWS || OS_LINUX
    struct stat buffer;
    if(stat(p.c_str(), &buffer) != 0) return false;
    return S_ISDIR(buffer.st_mode);
#	endif
    return false;
}

bool filesys::is_regular_file(const path & p)
{
	/*
	DWORD fileAttrib = GetFileAttributes(p.c_str());
	if (fileAttrib == INVALID_FILE_ATTRIBUTES) return false;
	return (fileAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0; //For now: not a directory -> file
	*/
#	if OS_WINDOWS || OS_LINUX
    struct stat buffer;
    if(stat(p.c_str(), &buffer) != 0) return false;
    return S_ISREG(buffer.st_mode);
#   else

#   endif
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
	/*
	FILETIME ftCreate, ftAccess, ftLastWrite;
	HANDLE hFile = CreateFile(p.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	if (0 != GetFileTime(hFile, &ftCreate, &ftAccess, &ftLastWrite)) return 0;
	CloseHandle(hFile);
	return filetime_to_timet(ftLastWrite);
	*/
#	if OS_WINDOWS || OS_LINUX
    struct stat buffer;
    if(stat(p.c_str(), &buffer) != 0) return false;
    return buffer.st_mtime;
#	else

#	endif
    return uint32(0);
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
	/*
	*/
#	if OS_WINDOWS
	return CreateDirectory(p.c_str(), 0) != 0;
#	elif OS_LINUX
    int32 stat = mkdir(p.c_str(), 777);
    return stat != 0;
#   else
#	endif
    return false;
}

bool filesys::exists(const path & p)
{
#	if OS_WINDOWS || OS_LINUX
    struct stat buffer;
    return stat(p.c_str(), &buffer) == 0;
#	else
	return false;
#	endif
}

filesys::DirectoryIterator::DirectoryIterator() :
	hdl(nullptr), current("")
{
}

int32 filter_dirEntry(const struct dirent* dir){
    return int32(strcmp(".", dir->d_name) != 0 && strcmp("..", dir->d_name) != 0);
}

filesys::DirectoryIterator::DirectoryIterator(const path & p, const path& filter /* = "*.*"*/) :
    hdl(nullptr),
    i(0), n(0),
    current(""), basePath("")
{
	/*
	WIN32_FIND_DATA ffd;
	path p2 = filesys::concat(p, filter);
	basePath = p;
	hdl = FindFirstFile(p2.c_str(), &ffd);
	if (hdl == INVALID_HANDLE_VALUE) 
		hdl = nullptr;
	current = filesys::concat(basePath,path(ffd.cFileName));
	if (path(ffd.cFileName) == "." || path(ffd.cFileName) == "..")
		operator ++();
	*/
#	if OS_WINDOWS || OS_LINUX
    basePath = p;
    i = 0;
    struct dirent **nameList = nullptr;
    n = scandir(p.c_str(),&nameList,filter_dirEntry, alphasort);
    if(n <= 0){
        hdl = nullptr;
        n = 0;
        return;
    }
    hdl = (void*)nameList;
    current = filesys::concat(basePath,filesys::path(nameList[i]->d_name));
    free(nameList[i++]);
#   else
#	endif
}

filesys::DirectoryIterator & filesys::DirectoryIterator::operator++()
{
	/*
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
	*/
#	if OS_WINDOWS || OS_LINUX
    struct dirent** nameList = (struct dirent**)hdl;
    if(i == n){
        current = path();
        free(nameList);
        hdl = nullptr;
        return *this;
    }
    current = filesys::concat(basePath, filesys::path(nameList[i]->d_name));
    free(nameList[i++]);
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
#	if OS_WINDOWS || OS_LINUX
    return hdl != nullptr;
#	else

#	endif
}

bool filesys::DirectoryIterator::operator==(const DirectoryIterator & rhs) const
{
#	if OS_WINDOWS || OS_LINUX
    return hdl == rhs.hdl && i == rhs.i && n == rhs.n;
#	else
#	endif
}

ENDNAMESPACE
