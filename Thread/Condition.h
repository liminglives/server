#ifndef CONDITION_H
#define CONDITION_H

#include "Mutex.h"

class Condition
{
public:
    Condition(Mutex &mutex_):mutex(mutex_)
    {
        pthread_cond_init(&cond_t, NULL);    
    }
    ~Condition()
    {
        pthread_cond_destory(&cond_t);
    }

    void wait()
    {
        pthread_cond_wait(&cond_t, mutex.getMutex());
    }

    void notify()
    {
        pthread_cond_signal(&cond_t);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&cond_t);
    }

    
private:
    Condition(const Condition &);
    Condition & operator=(const Condition &);
    
    pthread_cond_t cond_t;
    Mutex &mutex;
};

#endif
