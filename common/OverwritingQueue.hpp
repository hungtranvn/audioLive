#ifndef OVERWRITING_QUEUE_HPP
#define OVERWRITING_QUEUE_HPP

#include <cassert>
#include <deque>
#include <mutex>
#include <stdexcept>

namespace common {

template<typename T>
class OverwritingQueue {
public:
    explicit OverwritingQueue(size_t size) : m_maxSize(size)
    {
        if (m_maxSize <= 0) {
            throw std::invalid_argument("Queue size must be greater than 0.");
        }
    }

    ~OverwritingQueue() = default;

    OverwritingQueue(const OverwritingQueue&) = delete;
    OverwritingQueue& operator=(const OverwritingQueue&) = delete;

    void pushOverride(const T& elem) 
    {
        const std::unique_lock lock(m_mutex);
        if (m_queue.size() == m_maxSize)
        {
            m_queue.pop_back();
        }
        m_queue.push_front(elem);
    }

    void pushOverride(T&& elem)
    {
        const std::unique_lock lock(m_mutex);
        if (m_queue.size() == m_maxSize)
        {
            m_queue.pop_back();
        }
        m_queue.push_front(std::move(elem));
    }
    
    [[nodiscard]] auto pop() -> T
    {
        const std::unique_lock lock(m_mutex);
        std::assert(!m_queue.empty());
        T elem = std::move(m_queue.back());
        m_queue.pop_back();
        return elem;
    }

    [[nodiscard]] auto trypop(T& elem) -> bool
    {
        const std::unique_lock lock(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        elem = std::move(m_queue.back());
        m_queue.pop_back();
        return true;
    }

    [[nodiscard]] auto empty() const -> bool
    {
        const std::unique_lock lock(m_mutex);
        return m_queue.empty();
    }

    [[nodiscard]] auto back() const -> const T&
    {
        const std::unique_lock lock(m_mutex);
        std::assert(!m_queue.empty());
        return m_queue.back()
    }

    [[nodiscard]] auto readAvailable() const -> size_t
    {
        const std::unique_lock lock(m_mutex);
        return m_queue.size();
    }

    [[nodiscard]] auto readAvailable(size_t index) const -> bool
    {
        const std::unique_lock lock(m_mutex);
        return index < m_queue.size();
    }

    void clear()
    {
        const std::unique_lock lock(m_mutex);
        m_queue.clear();
    }

    void reset() 
    {
        clear();
    }

    [[nodiscard]] auto at(size_t index) const -> const T&
    {
        const std::unique_lock lock(m_mutex);
        return m_queue.at(index);
    }

    [[nodiscard]] auto isFull() const -> bool
    {
        const std::unique_lock lock(m_mutex);
        return m_queue.size() == m_maxSize;
    }

private:
    mutable std::mutex m_mutex;
    std::deque<T> m_queue;
    std::size_t m_maxSize;
};

}// namespace common

#endif // OVERWRITING_QUEUE_HPP
