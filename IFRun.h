#ifndef IFRUN_H
#define IFRUN_H

class IFRun
{
public:
    virtual void run(void *param) = 0;
    virtual ~IFRun(){};
};

#endif
