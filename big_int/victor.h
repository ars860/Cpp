#ifndef BIGINT_VICTOR_H
#define BIGINT_VICTOR_H

#include <cstddef>
#include <memory>
#include <vector>

struct victor {
    victor();

    ~victor(){
        if(is_big){
            data.big_data = nullptr;
        };
    };

    victor(victor const &other);

    victor(size_t size);

    victor(std::vector<unsigned int> const &vec);

    unsigned int &operator[](size_t pos);

    unsigned int const &operator[](size_t pos) const;

    size_t size() const;

    void resize(size_t new_size);

    void push_back(unsigned int val);

    void pop_back();

    unsigned int back() const;

    void swap(victor &other) noexcept;

    void print() const;

private:
    size_t capacity;
    static const size_t small_size = 8; //random number
    size_t sz;

//    std::shared_ptr<unsigned int> big_data;
//    unsigned int small_data[small_size];

    union Data {
        std::shared_ptr<std::vector<unsigned int> > big_data;
        unsigned int small_data[small_size]; //std::vector<unsigned int> small_data;   // don't forget to resize to small_size

        Data() {
//            big_data = nullptr;
            std::fill(small_data, small_data + small_size, 0);
        };

        ~Data(){};
    } data;

    void reserve(size_t new_size);

    bool is_big;

    void ensure_capacity(size_t pos);
};

#endif //BIGINT_VICTOR_H
