#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() {}

    // 阻塞式入队操作
    void enqueue(const T &item)
    {
        std::lock_guard<std::mutex> lock(mtx); // 保护对队列的访问
        q.push(item);
        cv.notify_one(); // 通知等待线程
    }

    // 阻塞式出队操作
    T dequeue()
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]()
                { return !q.empty(); }); // 等待队列非空
        T item = q.front();
        q.pop();
        return item;
    }

    // 非阻塞的检查队列是否为空
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

    // 获取队列的大小
    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }

private:
    std::queue<T> q;
    mutable std::mutex mtx;     // 用于保护队列的互斥量
    std::condition_variable cv; // 用于线程间的通知
};