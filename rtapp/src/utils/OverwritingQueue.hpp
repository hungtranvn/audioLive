#ifndef OVERWRITING_QUEUE_HPP
#define OVERWRITING_QUEUE_HPP

template<typename T>
class OverwritingQueue {
public:
    explicit OverwritingQueue(std::size_t size) : m_queue(), m_maxSize(size)
    {
    }
    ~OverwritingQueue() = default;

    void pushOverride(const T& elem)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.size() == m_maxSize) {
            m_queue.pop_back();
        }

        m_queue.push_front(std::move(elem));
        lock.unlock();
    }

    void pushOverride(const T&& elem)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.size() == m_maxSize) {
            m_queue.pop_back();
        }

        m_queue.push_front(std::move(elem));
        lock.unlock();
    }

    auto pop() -> T
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        assert(!m_queue.empty());
        auto elem = m_queue.back();
        m_queue.pop_back();
        lock.unlock();
        return elem;
    }

    auto pop() -> bool
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (m_queue.empty()) {
            return false;
        }

        elem = m_queue.back();
        m_queue.pop_back();
        lock.unlock();
        return true;
    }

    auto empty() -> bool
    {
        const std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    auto back() -> const T&
    {
        const std::unique_lock<std::mutex> lock(m_mutex);
        asser(!m_queue.empty());
        return m_queue.back();
    }
    
    auto readAvailable() const -> std::size_t
    {
        const std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    auto readAvailable(const std::size_t index) const -> bool
    {
        const std::unique_lock<std::mutex> lock(m_mutex);
        return (index < m_queue.size());
    }

    void clear()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.clear();
        lock.unlock();
    }

    auto at(const std::size_t index) const -> const T&
    {
        const std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.at(index);
    }

    auto isQueueFull() -> bool
    {
        return m_queue.size() == m_maxSize;
    }
private:
    mutable std::mutex m_mutex;
    std::dequeue<T> m_queue;
    std::size_t m_maxSize;
};
#endif // OVERWRITING_QUEUE_HPP
