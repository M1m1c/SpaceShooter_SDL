#pragma once
#include <cstddef>
#include <array>
#include <memory>

template<typename T, std::size_t N>
class FixedSizeSet {
public:
    using value_type = T;
    using size_type = std::size_t;

    FixedSizeSet() = default;

    void insert(const value_type& value) 
    {
        if (contains(value)) return;

        for (size_type i = 0; i < N; ++i) {
            if (m_Data[i] == nullptr) {
                m_Data[i] = std::make_unique<value_type>(value);
                return;
            }
        }

        //TODO set is full deal with it
    }

    void erase(const value_type& value) 
    {
        for (size_type i = 0; i < N; ++i)
        {
            if (m_Data[i] != nullptr && *m_Data[i] == value) 
            {
                m_Data[i] = nullptr;
                return;
            }
        }
    }

    size_type size() const 
    {
        size_type count = 0;
        for (size_type i = 0; i < N; ++i) 
        {
            if (m_Data[i] != nullptr) ++count;
        }
        return count;
    }

    size_type max_size() const { return N; }

    bool contains(const value_type& value) const 
    {
        bool retval = false;
        for (size_type i = 0; i < N; ++i) 
        {
            if (m_Data[i] != nullptr && *m_Data[i] == value) 
            {
                retval = true;
                break;
            }
        }
        return retval;
    }

    auto begin() const
    {
        return m_Data.begin();
    }

    auto end() const
    {
        return m_Data.end();
    }

private:
    std::array<std::unique_ptr<T>, N> m_Data;
};