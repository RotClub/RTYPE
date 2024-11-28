/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** SafeQueue
*/

#ifndef SAFEQUEUE_HPP_
    #define SAFEQUEUE_HPP_

    #include <queue>
    #include <mutex>
    #include <condition_variable>

template <typename T>
class SafeQueue {
    public:
        SafeQueue() = default;
        SafeQueue(const SafeQueue &other) = default;
        ~SafeQueue() = default;

        void enqueue(T t);
        T dequeue();
        bool empty();

    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _cond;
};

#endif /* !SAFEQUEUE_HPP_ */