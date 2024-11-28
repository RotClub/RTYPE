/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** SafeQueue
*/

#include "SafeQueue.hpp"

template <typename T>
void SafeQueue<T>::enqueue(T t)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.push(t);
    _cond.notify_one();
}

template <typename T>
T SafeQueue<T>::dequeue()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty()) {
            _cond.wait(lock);
        }
        T val = _queue.front();
        _queue.pop();
        return val;
    }
}

template <typename T>
bool SafeQueue<T>::empty()
{
    return _queue.empty();
}
