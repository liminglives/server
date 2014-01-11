#include "Buffer.h"

Buffer::Buffer()
{}

Buffer::~Buffer()
{}

bool Buffer::empty()
{
    return buf.empty();
}

const char * Buffer::toCString()
{
    return buf.c_str();
}

size_t Buffer::readableBytes()
{
    return (buf.size());
}

void Buffer::append(const string & data)
{
    buf.append(data);
}

void Buffer::append(const char * data, int len)
{
    buf.append(data, len);
}

void Buffer::clear()
{
    buf.clear();
}

void Buffer::retrive(size_t n)
{
    if (n >= readableBytes())
        buf.clear();
    else
        buf = buf.substr(n, buf.size());
}

string Buffer::retriveAsAllString()
{
    return retriveAsString(readableBytes());
}

string Buffer::retriveAsString(size_t n)
{
    string ret;
    if (n >= buf.size())
    {
        ret = buf;
        buf.clear();
    }
    else
    {
        ret = buf.substr(0, n);
        buf = buf.substr(n, buf.size());
    }
    return ret;
}