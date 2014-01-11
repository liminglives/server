#ifndef BUFFER_H
#define BUFFER_H

#include <string>
using namespace std;

class Buffer
{
public:
    Buffer();
    ~Buffer();
    bool empty();
    size_t readableBytes();
    void append(const string &data);
    void append(const char *, int len);
    void clear();
    string retriveAsAllString();
    string retriveAsString(size_t n);
    void retrive(size_t n);
    const char * toCString();

private:
    string buf;
};

#endif