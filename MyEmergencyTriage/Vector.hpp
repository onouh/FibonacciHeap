#ifndef VECTOR_HPP
#define VECTOR_HPP

//#include <stdexcept>
#include <algorithm>

template <typename T>
class Vector {
private:
    T* data;
    size_t capacity_size;
    size_t current_size;

    void reallocate(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < current_size; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        capacity_size = new_cap;
    }

public:
    Vector() : data(nullptr), capacity_size(0), current_size(0) {
        reallocate(2);
    }

    // Constructor for specific size with default values (used in consolidate)
    Vector(size_t n, const T& val = T()) : data(new T[n]), capacity_size(n), current_size(n) {
        for (size_t i = 0; i < n; ++i) {
            data[i] = val;
        }
    }

    ~Vector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (current_size == capacity_size) {
            reallocate(capacity_size * 2);
        }
        data[current_size++] = value;
    }

    void resize(size_t new_size, const T& val = T()) {
        if (new_size > capacity_size) {
            reallocate(new_size);
        }
        for (size_t i = current_size; i < new_size; ++i) {
            data[i] = val;
        }
        current_size = new_size;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const { return current_size; }

    // Iterators for range-based for loops (used in consolidate)
    T* begin() { return data; }
    T* end() { return data + current_size; }
    const T* begin() const { return data; }
    const T* end() const { return data + current_size; }
};

#endif
