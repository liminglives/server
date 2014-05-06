#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class Mutex
{
public:
    Mutex() 
    {
        pthread_mutex_init(&mutex_t, NULL);
    }
    ~Mutex()
    {
        pthread_mutex_destory(&mutex_t);
    }

    int lock()
    {
        return pthread_mutex_lock(&mutex_t);
    }
    //void tryLock();
    int unlock();
    {
        return pthread_mutex_unlock(&mutex_t);
    }

    pthread_mutex_t * getMutex()
    {
        return &mutex_t;
    }
    
private:
    Mutex(const Mutex&);
    Mutex & operator=(const Mutex&);

    pthread_mutex_t mutex_t;
};

class MutexGuard
{
public:
    MutexGuard(Mutex &mutex_):mutex(mutex_)
    {
        mutex.lock();
    }

    ~MutexGuard()
    {
        mutex.unlock;
    }

private:
    MutexGuard(const MutexGuard &);
    MutexGuard & operator=(cosnt MutexGuard &);
    
    Mutex &mutex;
};

#endif