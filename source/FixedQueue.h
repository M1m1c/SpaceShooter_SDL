#pragma once
#include <stdexcept>

template <typename T, size_t MaxSize>
class FixedQueue {
public:
    void push(const T& value) {
        if (m_size == MaxSize) {
            throw std::length_error("FixedQueue is full!");
        }

        m_data[m_tail] = value;
        m_tail = (m_tail + 1) % MaxSize;
        ++m_size;
    }

    void pop() {
        if (m_size == 0) {
            throw std::length_error("FixedQueue is empty!");
        }

        m_head = (m_head + 1) % MaxSize;
        --m_size;
    }

    const T& front() const {
        if (m_size == 0) {
            throw std::length_error("FixedQueue is empty!");
        }

        return m_data[m_head];
    }

    const T& back() const {
        if (m_size == 0) {
            throw std::length_error("FixedQueue is empty!");
        }

        return m_data[(m_tail + MaxSize - 1) % MaxSize];
    }

    size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

private:
    T m_data[MaxSize]{};
    size_t m_head = 0;
    size_t m_tail = 0;
    size_t m_size = 0;
};