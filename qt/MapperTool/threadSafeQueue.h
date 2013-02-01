#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <QQueue>
#include <QSemaphore>
#include <QMutex>

template<class T> class ThreadSafeQueue
{
public:
    ThreadSafeQueue(int maxSize = 64) : _semFree(maxSize), _semUsed(0)
    {

    }

    void enqueue(const T &value) {
        _semFree.acquire(1);
        _mutex.lock();
        _queue.enqueue(value);
        _mutex.unlock();
        _semUsed.release(1);
    }

    T dequeue() {
        _semUsed.acquire(1);
        _mutex.lock();
        T val = _queue.dequeue();
        _mutex.unlock();
        _semFree.release(1);
        return val;
    }

    T head() {
        _semUsed.acquire(1);
        _mutex.lock();
        T val = _queue.head();
        _mutex.unlock();
        _semUsed.release(1);
    }

    bool isEmpty() const {
        return _semUsed.available()==0;
    }

    bool isFull() const {
        return _semFree.available()==0;
    }

    int count() const {
        return _semUsed.available();
    }

    T tryHead() {
        bool t = _semUsed.tryAcquire(1);
        if(!t)
            return T();
        _mutex.lock();
        T val = _queue.head();
        _mutex.unlock();
        _semUsed.release();
        return val;
    }

    T tryDequeue() {
        bool t = _semUsed.tryAcquire(1);
        if(!t)
            return T();
        _mutex.lock();
        T val = _queue.dequeue();
        _mutex.unlock();
        _semUsed.release();
        return val;
    }

private:
    QQueue<T> _queue;
    QSemaphore _semFree;
    QSemaphore _semUsed;
    QMutex _mutex;
};

#endif // THREADSAFEQUEUE_H
