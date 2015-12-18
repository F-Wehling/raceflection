#pragma once

BEGINNAMESPACE

class ByteBuffer : public std::basic_streambuf<ansichar> {
public:
	ByteBuffer(const Byte* data, size_type size) {
		setg((ansichar*)data, (ansichar*)data, (ansichar*)data + size);
	}
};

class ByteStream : public std::istream {
public:
	ByteStream(const Byte* data, size_type size) : 
		std::istream(&m_Buffer),
		m_Buffer(data, size) {			
		rdbuf(&m_Buffer);
	}
private:
	ByteBuffer m_Buffer;
};

ENDNAMESPACE