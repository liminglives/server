#ifndef SHAREABLE_H
#define SHAREABLE_H

class Shareable
{
public:
    Shareable():_count(0)
    {}
    virtual ~Shareable()
    {}
    void inc()
    {
        MutexGuard(_mutex);
        ++_count;
    }
    int dec()
    {
        MutexGuard(_mutex);
        return _count>0 ? --_count : _count;
    }
    int getCount()
    {
        MutexGuard(_mutex);
        return _count;
    }
private:
    int _count;
    Mutex _mutex;

};

#endif
