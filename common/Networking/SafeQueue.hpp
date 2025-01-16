// SafeQueue.hpp

#ifndef SAFEQUEUE_HPP_
#define SAFEQUEUE_HPP_

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class SafeQueue
{
    public:
        SafeQueue() = default;
        SafeQueue(const SafeQueue &other) = default;
        ~SafeQueue() = default;

        void enqueue(T t)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _queue.push(t);
            _cond.notify_one();
        }

        T dequeue()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while (_queue.empty()) {
                _cond.wait(lock);
            }
            T val = _queue.front();
            _queue.pop();
            return val;
        }

        bool empty()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _queue.empty();
        }

        int size()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _queue.size();
        }

    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _cond;
};

#endif /* !SAFEQUEUE_HPP_ */
