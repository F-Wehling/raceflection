#pragma once

BEGINNAMESPACE

class ByteBuffer : public std::streambuf {
    typedef ansichar value_type;
    typedef std::streambuf parent;
    typedef typename parent::traits_type traits_type;
    typedef typename parent::int_type int_type;
public:
    inline ByteBuffer() : begin_(nullptr), end_(nullptr), current_(nullptr) {

    }

    inline ByteBuffer(const value_type* begin, const value_type* end) :
        begin_(begin),
        end_(end),
        current_(begin_)
    {
    }

    inline ByteBuffer(const Byte* begin, const Byte *end) :
        begin_((value_type*)begin),
        end_((value_type*)end),
        current_(begin_)
    {
    }

    inline ByteBuffer(const value_type *begin, size_type size) :
        begin_(begin),
        end_(begin + size),
        current_(begin_)
    {}

    inline ByteBuffer(const Byte* begin, size_type size) :
        begin_((value_type*)begin),
        end_((value_type*)(begin + size)),
        current_(begin_){

    }

private:
    inline int_type underflow() {
        if(current_ == end_) return traits_type::eof();
        return traits_type::to_int_type(*current_);
    }

    inline int_type uflow() {
        if(current_ == end_) return traits_type::eof();
        return traits_type::to_int_type(*current_++);
    }

    inline int_type pbackfail(int32 ch) {
        if(current_ == begin_ || (ch != traits_type::eof() && ch != current_[-1]))
            return traits_type::eof();
        return traits_type::to_int_type(*--current_);
    }

    inline std::streamsize showanyc(){
        return end_ - current_;
    }

    ByteBuffer(const ByteBuffer& rhs) = delete;
    ByteBuffer& operator = (const ByteBuffer& rhs) = delete;

private:
    const value_type* const begin_;
    const value_type* const end_;
    const value_type* current_;
};

class ByteStream : public std::istream {
    typedef ansichar value_type;
    typedef std::istream parent;
public:
    ByteStream() : parent(&m_Buffer), m_Buffer() {}

    ByteStream(const value_type* begin, const value_type* end) :
        parent(&m_Buffer),
        m_Buffer(begin, end)
    {
    }

    ByteStream(const value_type* data, size_type size) :
        parent(&m_Buffer),
        m_Buffer(data, size)
    {
    }

    ByteStream(const Byte* begin, const Byte* end) :
        parent(&m_Buffer),
        m_Buffer(begin, end) {}

    ByteStream(const Byte* data, size_type size) :
        parent(&m_Buffer),
        m_Buffer(data, size){}

private:
    ByteBuffer m_Buffer;
};


ENDNAMESPACE
