#ifndef SHARE_POINT_H
#define SHARE_POINT_H

// the type must inherit from shareable class
template <typename T>
class SharePoint
{
public:
    SharePoint(T *p=NULL) : _pointer(p)
    {
        if (NULL != _pointer)
            _pointer.inc();
    }
    SharePoint(const SharePoint<T> &sh) : _pointer(sh._pointer)
    {
        if (NULL != _pointer)
            _pointer.inc();
    }

    SharePoint & operator=(const SharePoint<T> &sh)
    {
        if (sh._pointer != _pointer)
        {
            if (_pointer.dec() == 0)
            {
                delete _pointer;
            }
            _pointer = sh._pointer;
            if (_pointer != NULL)
                _pointer.inc();
        }

        return *this;
    }

    template <typename S>
    SharePoint & operator=(const SharePoint<S> &sh)
    {
        
    }
    
    ~SharePoint(){}
private:
    T * _pointer;
};

#endif