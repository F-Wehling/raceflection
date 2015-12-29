#pragma once

BEGINNAMESPACE

template<typename Byte_type>
class TByteBuffer : public std::basic_streambuf<Byte_type> {
    typedef Byte_type value_type;
    typedef std::basic_streambuf<value_type> parent;
    typedef typename parent::traits_type traits_type;
    typedef typename parent::int_type int_type;
public:
    inline TByteBuffer(const value_type* begin, const value_type* end) :
        parent(),
        begin_(begin),
        end_(end),
        current_(begin_)
    {
    }

    inline TByteBuffer(const value_type *begin, size_type size) :
        parent(),
        begin_(begin),
        end_(begin + size),
        current_(begin_)
    {}

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

private:
    const value_type* begin_;
    const value_type* end_;
    const value_type* current_;
};

template<typename Byte_type>
class TByteStream : public std::basic_istream<Byte_type> {
    typedef Byte_type value_type;
    typedef std::basic_istream<value_type> parent;
public:
    TByteStream(const value_type* begin, const value_type* end) :
        parent(&m_Buffer),
        m_Buffer(begin, end)
    {
    }

    TByteStream(const value_type* data, size_type size) :
        parent(&m_Buffer),
        m_Buffer(data, size)
    {
    }

private:
    TByteBuffer<value_type> m_Buffer;
};

typedef TByteStream<Byte> ByteStream;
typedef TByteStream<ansichar> CharStream;

typedef TByteBuffer<Byte> ByteBuffer;
typedef TByteBuffer<ansichar> CharBuffer;

ENDNAMESPACE
