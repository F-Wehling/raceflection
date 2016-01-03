#pragma once

#include "Container/Array.h"

BEGINNAMESPACE

class filesys {
public:
	typedef String path;

public:
	static path extension(const path& p);
	static path concat(const path& p1, const path& p2);
	static size_type file_size(const path& p);
	static bool is_directory(const path& p);
	static bool is_regular_file(const path& p);
	static uint32 last_write_time(const path& p);
	static path stem(const path& p);
	static bool create_directory(const path& p);
	static bool exists(const path& p);
public:
	class DirectoryIterator {
	public:
		DirectoryIterator();
		DirectoryIterator(const path& p, const path& filter = "*.*");
		DirectoryIterator& operator ++ ();
		const path& operator * () const;
		operator bool() const;
		bool operator == (const DirectoryIterator& rhs) const;
	private:
		void* hdl;
        int32 i,n;
		path current;
		path basePath;
	};
};

ENDNAMESPACE
