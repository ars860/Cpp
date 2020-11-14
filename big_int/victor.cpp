#include <cstring>
#include <iostream>
#include "victor.h"

victor::victor() : sz(0), is_big(false), capacity(666), data() {
}

victor::victor(size_t size) {
    if (size > small_size) {
        is_big = true;
        data.big_data = std::make_shared<std::vector<unsigned int> >(size);
        sz = size;
    } else {
        sz = size;
        is_big = false;
    }
}

victor::victor(victor const &other) {
    is_big = other.is_big;
    if (is_big) {
        data.big_data = other.data.big_data;
        capacity = other.capacity;
        sz = other.sz;
    } else {
        std::copy(other.data.small_data, other.data.small_data + small_size, data.small_data);
        sz = other.sz;
        is_big = false;
    }
}

victor::victor(std::vector<unsigned int> const &vec) {
    is_big = vec.size() >= small_size;
    if (is_big) {
        data.big_data = std::make_shared<std::vector<unsigned int> >(vec);
        capacity = vec.size();
        sz = vec.size();
    } else {
        std::copy(vec.begin(), vec.end(), data.small_data);
        sz = vec.size();
        is_big = false;
    }
}

unsigned int &victor::operator[](size_t pos) {
    if (is_big) {
        if(!data.big_data.unique()){
            std::vector<unsigned int> old_big_data(*data.big_data.get());
            data.big_data = std::make_shared<std::vector<unsigned int> >(old_big_data);
        }
        return (*data.big_data.get())[pos];
    } else {
        return data.small_data[pos];
    }
}

unsigned int const &victor::operator[](size_t pos) const {
    if (is_big) {
        return (*data.big_data.get())[pos];
    } else {
        return data.small_data[pos];
    }
}

unsigned int victor::back() const {
    if (is_big) {
        return (*data.big_data.get())[sz - 1];
    } else {
        return data.small_data[sz - 1];
    }
}

size_t victor::size() const {
    return sz;
}

void victor::reserve(size_t new_size) {
    if (is_big) {
        std::vector<unsigned int> tmp = *data.big_data.get();
        tmp.resize(new_size);

        data.big_data = std::make_shared<std::vector<unsigned int>>(tmp);
        capacity = new_size;
    } else {
        if (new_size >= small_size) {
            is_big = true;

            std::vector<unsigned int> new_data(data.small_data, data.small_data + small_size);
            new_data.resize(new_size);

            std::fill(data.small_data, data.small_data + small_size, 0);
            data.big_data = std::make_shared<std::vector<unsigned int> >(new_data);
            capacity = new_size;
        }
    }
}

void victor::resize(size_t new_size) {
    reserve(new_size);
    sz = new_size;
}

void victor::ensure_capacity(size_t pos) {  //try to modify data[pos], if pos > capacity  =>  capacity *= 2
    if (is_big && pos >= capacity)
        reserve(capacity * 2);
    if (!is_big && pos >= small_size)
        reserve(2 * small_size);
}

void victor::push_back(unsigned int val) {
    ensure_capacity(sz);
    if (is_big) {
        (*data.big_data.get())[sz] = val;
        sz++;
    } else {
        data.small_data[sz] = val;
        sz++;
    }
}

void victor::pop_back() {
    if (sz != 0)
        sz--;
}

void victor::swap(victor &other) noexcept {
    std::swap(is_big, other.is_big);
    std::swap(capacity, other.capacity);
    std::swap(sz, other.sz);
    std::swap(data.small_data, other.data.small_data);
}

void victor::print() const {
    if (is_big) {
        for (int i = 0; i < sz; i++) {
            std::cout << (*data.big_data.get())[i] << " ";
        }
    } else {
        for (int i = 0; i < sz; i++) {
            std::cout << data.small_data[i] << " ";
        }
    }
    std::cout << '\n';
}
