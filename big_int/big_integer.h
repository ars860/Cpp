#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <cstddef>
#include <iosfwd>
#include <vector>
#include "victor.h"

struct big_integer {
    big_integer();

    big_integer(big_integer const &other);

    big_integer(int a);

    big_integer(unsigned int a);

    big_integer(std::vector<unsigned int> const &vec);

    big_integer(int sgn, std::vector<unsigned int> const &vec);

    explicit big_integer(std::string const &str);

    big_integer &operator=(big_integer const &other);

    big_integer &operator+=(big_integer const &rhs);

    big_integer &operator-=(big_integer const &rhs);

    big_integer &operator*=(big_integer const &rhs);

    big_integer &operator/=(big_integer const &rhs);

    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &rhs);

    big_integer &operator|=(big_integer const &rhs);

    big_integer &operator^=(big_integer const &rhs);

    big_integer &operator<<=(int rhs);

    big_integer &operator>>=(int rhs);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~() const;

    big_integer &operator++();

    big_integer operator++(int);

    big_integer &operator--();

    big_integer operator--(int);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend std::string to_string(big_integer const &a);

    void swap(big_integer &other);

    bool is_zero() const;

    void stupid_print();

private:
    int sign;

    void two_complainment(unsigned int sz);

    victor data;

    void resize(int x);

    friend int compare(big_integer const &first, big_integer const &second); // 0 - =, 1 - <, 2 - >

    void remove_extra_zeros();

    void shifted_add_data(big_integer &first, big_integer const &second, int shift);

    void add_data(big_integer &first, big_integer const &second, bool for_div = 0);

    void sub_data(big_integer &first, big_integer const &second, bool for_div = 0);

    unsigned int size();

    void mul_small(big_integer &res, const big_integer &a, const unsigned int b);

    template<typename FunctorT>
    big_integer apply_functor(big_integer const & rhs, FunctorT functor);
};

big_integer operator+(big_integer a, big_integer const &b);

big_integer operator-(big_integer a, big_integer const &b);

big_integer operator*(big_integer a, big_integer const &b);

big_integer operator/(big_integer a, big_integer const &b);

big_integer operator%(big_integer a, big_integer const &b);

big_integer operator&(big_integer a, big_integer const &b);

big_integer operator|(big_integer a, big_integer const &b);

big_integer operator^(big_integer a, big_integer const &b);

big_integer operator<<(big_integer a, int b);

big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const &a, big_integer const &b);

bool operator!=(big_integer const &a, big_integer const &b);

bool operator<(big_integer const &a, big_integer const &b);

bool operator>(big_integer const &a, big_integer const &b);

bool operator<=(big_integer const &a, big_integer const &b);

bool operator>=(big_integer const &a, big_integer const &b);

std::string to_string(big_integer const &a);

std::ostream &operator<<(std::ostream &s, big_integer const &a);

#endif // BIG_INTEGER_H