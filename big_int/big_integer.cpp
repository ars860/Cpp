#include "big_integer.h"

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <iostream>

unsigned int base2 = 32;
int base10 = 1000000000;
int cell_size = 10;
unsigned long long MAX = (unsigned long long) UINT32_MAX + (unsigned long long) 1;

big_integer::big_integer()
        : big_integer(0, {0}) {}

big_integer::big_integer(big_integer const &other) : data(other.data), sign(other.sign) {
}

big_integer::big_integer(std::vector<unsigned int> const &vec)
        : big_integer(1, vec) {}

big_integer::big_integer(int sgn, std::vector<unsigned int> const &vec)
        : data(vec), sign(sgn) {}

big_integer::big_integer(int a) : sign(a < 0 ? -1 : 1), data(1) {
    if (a == INT32_MIN) {
        data[0] = (unsigned int) INT32_MAX + 1;
    } else {
        data[0] = (unsigned int) abs(a);
    }
    if (a == 0) {
        sign = 0;
    }
    remove_extra_zeros();
}

big_integer::big_integer(unsigned int a) : sign(a == 0 ? 0 : 1), data(1) {
    data[0] = a;
}

int parseInt(std::string str) {
    return std::stoi(str);
}

big_integer::big_integer(std::string const &str) {
    big_integer ans = big_integer(0);
    int newsign = (str[0] == '-') ? -1 : 1;
    unsigned int pos = (newsign == -1) ? 1 : 0;

    for (; pos + cell_size - 1 <= str.size(); pos += cell_size - 1) {
        ans = ans * big_integer(base10) +
              big_integer(parseInt(str.substr(pos, cell_size - 1)));
    }

    if (pos < str.size()) {
        unsigned int pow = 1;
        for (int i = 0; i < (str.size() - ((newsign == -1) ? 1 : 0)) % (cell_size - 1); i++) {
            pow *= 10;
        }
        ans = ans * big_integer(pow) + big_integer(parseInt(str.substr(pos, cell_size - 1)));
    }
    *this = ans;
    this->sign = newsign;
}

void big_integer::stupid_print() {
    if (sign == 1)
        std::cout << "+ ";
    if (sign == -1)
        std::cout << "- ";
    for (int i=0;i<data.size();i++) {
        std::cout << data[i] << " ";
    }
    std::cout << '\n';
}

void big_integer::swap(big_integer &other) {
    this->data.swap(other.data);
    std::swap(this->sign, other.sign);
}

big_integer &big_integer::operator=(big_integer const &other) {
    big_integer tmp = other;
    swap(tmp);
    return *this;
}

bool big_integer::is_zero() const {
    return (sign == 0 || (data.size() == 1 && data[0] == 0));
}

unsigned int get(victor const &x, int pos) {
    if (pos >= x.size() || pos < 0) {
        return 0;
    }
    return x[pos];
}


void big_integer::add_data(big_integer &first, big_integer const &second, bool for_div) {
    unsigned int carry = 0;
    unsigned long long sum;
    first.data.resize(std::max(first.data.size(), second.data.size()));
    for (int i = 0; i < first.data.size(); i++) {
        sum = (unsigned long long) get(first.data, i) + (unsigned long long) get(second.data, i) +
              (unsigned long long) carry;
        first.data[i] = (unsigned int) (sum & (MAX - 1));
        carry = (unsigned int) (sum >> base2);
    }
    if (carry > 0) {
        data.push_back(carry);
    }
    if (!for_div)
        first.remove_extra_zeros();
}

void big_integer::shifted_add_data(big_integer &first, big_integer const &second, int shift) {
    big_integer tmp;
    tmp.resize(second.data.size() + shift);
    for (int i = 0; i < second.data.size(); i++) {
        tmp.data[i + shift] = second.data[i];
    }
    add_data(first, tmp);
}

void big_integer::resize(int x) {
    data.resize(x);
}

void big_integer::remove_extra_zeros() {
    while (data.back() == 0 && data.size() > 1) {
        data.pop_back();
    }
    if (is_zero()) {
        sign = 0;
    }
}

void big_integer::sub_data(big_integer &first, big_integer const &second, bool for_div) {
    unsigned int carry = 0, newcarry;
    unsigned long long sum, add = 0;
    first.data.resize(std::max(first.data.size(), second.data.size()));
    for (int i = 0; i < first.data.size(); i++) {
        if ((unsigned long long) get(first.data, i) <
            (unsigned long long) get(second.data, i) + (unsigned long long) carry) {
            newcarry = 1;
            add = MAX;
        } else {
            newcarry = 0;
            add = 0;
        }
        sum = (unsigned long long) get(first.data, i) + add - (unsigned long long) get(second.data, i) -
              (unsigned long long) carry;
        first.data[i] = (unsigned int) (sum & (MAX - 1));
        carry = newcarry;
    }
    if (carry) {
        int sz = first.data.size() + 1;
        big_integer tmp = big_integer(first);
        first = big_integer();
        first.resize(sz);
        first.sign = tmp.sign;
        first.data[sz - 1] = 1;
        sub_data(first, tmp);
        first.sign = -first.sign;
    }
    if (!for_div)
        first.remove_extra_zeros();
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    if (rhs.is_zero()) {
        return *this;
    }
    if (is_zero()) {
        *this = rhs;
        return *this;
    }
    if (sign == rhs.sign) {
        add_data(*this, rhs);
        return *this;
    }
    this->sign = -this->sign;
    *this -= rhs;
    this->sign = -this->sign;
    remove_extra_zeros();
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    if (rhs.is_zero()) {
        return *this;
    }
    if (is_zero()) {
        *this = -rhs;
        return *this;
    }
    if (sign == rhs.sign) {
        sub_data(*this, rhs);
        return *this;
    }
    this->sign = -this->sign;
    *this += rhs;
    this->sign = -this->sign;
    remove_extra_zeros();
    return *this;
}

void big_integer::mul_small(big_integer &res, big_integer const &a, const unsigned int b) {
    unsigned int n = a.data.size();
    res.resize(n + 1);
    unsigned long long carry = 0, mul = 0, tmp = 0;
    for (size_t i = 0; i < n; i++) {
        mul = (unsigned long long) (a.data[i]) * b;
        tmp = (mul & (MAX - 1)) + carry;
        res.data[i] = (unsigned int) (tmp);
        carry = (mul >> base2) + (tmp >> base2);
    }
    res.data[n] = (unsigned int) (carry);
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    if (is_zero() || rhs.is_zero()) {
        *this = big_integer(0);
        return *this;
    }
    big_integer initial_this = big_integer(*this);
    int newsign = (sign != rhs.sign) ? -1 : 1;
    *this = big_integer(0);
    for (int i = 0; i < rhs.data.size(); i++) {
        big_integer tmp(initial_this);
        mul_small(tmp, tmp, rhs.data[i]);
        shifted_add_data(*this, tmp, i);
    }
    sign = newsign;
    return *this;
}

unsigned int trial(const unsigned int a, const unsigned int b, const unsigned int c) {
    unsigned long long res = a;
    res = ((res << base2) + b) / c;
    if (res > MAX - 1) {
        res = MAX - 1;
    }
    return (unsigned int) res;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    if (is_zero()) {
        *this = big_integer(0);
        return *this;
    }
    big_integer tmp_rhs(rhs);
    tmp_rhs.remove_extra_zeros();
    int newsign = tmp_rhs.sign == sign ? 1 : -1;
    sign = 1;
    tmp_rhs.sign = 1;

    if (*this < tmp_rhs) {
        *this = big_integer(0);
        return *this;
    }

    unsigned int f = (unsigned int) (MAX / ((unsigned long long) tmp_rhs.data.back() + 1));
    unsigned int n = size(), m = tmp_rhs.size();
    *this *= f;
    tmp_rhs *= f;

    unsigned int len = n - m + 1;
    unsigned int divider = tmp_rhs.data.back();
    big_integer ans(newsign, std::vector<unsigned int>(len));
    big_integer dividend(std::vector<unsigned int>(m + 1)), temp(std::vector<unsigned int>(m + 1));
    for (size_t i = 0; i < m; i++) {
        dividend.data[i] = get(data, n + i - m);
    }
    dividend.data[m] = get(data, n);

    for (size_t i = 0; i < len; i++) {
        dividend.data[0] = get(data, n - m - i);
        unsigned int curr_digit_place = len - 1 - i;
        unsigned int curr_digit = trial(dividend.data[m], dividend.data[m - 1], divider);
        mul_small(temp, tmp_rhs, curr_digit);
        while ((curr_digit >= 0) && dividend < temp) {
            temp -= tmp_rhs;
            curr_digit--;
        }
        sub_data(dividend, temp, 1);
        for (unsigned int j = m; j > 0; j--) {
            dividend.data[j] = dividend.data[j - 1];
        }
        ans.data[curr_digit_place] = curr_digit;
    }

    *this = ans;
    remove_extra_zeros();
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    *this = *this - (*this / rhs) * rhs;
    return *this;
}

void big_integer::two_complainment(unsigned int sz) {
    if (sign == -1) {
        for (int i = 0; i < size(); i++) {
            data[i] = (data[i] ^ UINT32_MAX);        // from -x make dodvux
        }
        sign = 1;
        *this += 1;
        data.push_back(UINT32_MAX);
        while (size() < sz) {
            data.push_back(UINT32_MAX);
        }
    } else {
        for (int i = 0; i < size(); i++) {
            data[i] = (data[i] ^ UINT32_MAX);        // from dodvux make -x
        }
        *this += 1;
        sign = -1;
    }
}

// std::bit_or, std::bit_and, std::bit_xor
// template<typename FunctorT>
// big_integer apply_functor(big_integer const & lhs, big_integer const & rhs, FunctorT functor)
//

template<typename FunctorT>
big_integer big_integer::apply_functor(big_integer const &rhs, FunctorT functor) {
    big_integer newlhs(*this), newrhs(rhs);

    unsigned int maxsz = std::max(newlhs.data.size(), rhs.data.size());

    if (newlhs.sign == -1)
        newlhs.two_complainment(maxsz);
    if (newrhs.sign == -1)
        newrhs.two_complainment(maxsz);

    unsigned int sz = std::max(newlhs.data.size(), newrhs.data.size());
    newlhs.resize(sz);
    for (int i = 0; i < sz; i++) {
        newlhs.data[i] = functor(get(newlhs.data, i), get(newrhs.data, i));
    }
    newlhs.sign =
            (newlhs.data.back() >> (base2 - 1)) == 1 ? -1 : 1;


    if (newlhs.sign == -1) {
        newlhs.sign = 1;
        newlhs.two_complainment(newlhs.data.size());
        newlhs.sign = -1;
    }
    newlhs.remove_extra_zeros();
    *this = newlhs;
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    apply_functor(rhs, std::bit_and<unsigned int>());
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    apply_functor(rhs, std::bit_or<unsigned int>());
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    apply_functor(rhs, std::bit_xor<unsigned int>());
    return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
    if (rhs == 0) {
        return *this;
    }
    if (sign == -1) {
        two_complainment(size());
    }

    unsigned int full_cells = rhs >> 5;
    unsigned int left = rhs % base2;
    unsigned int newsize = data.size() + full_cells + 1;
    std::vector<unsigned int> tmp(newsize);
    tmp[full_cells] = (unsigned int) (((unsigned long long) (get(data, 0)) << left) & (MAX - 1));
    for (size_t i = full_cells + 1; i < newsize; i++) {
        unsigned long long right_half = (unsigned long long) (get(data, i - full_cells)) << left;
        unsigned long long left_half = (unsigned long long) (get(data, i - full_cells - 1)) >> (base2 - left);
        tmp[i] = (unsigned int) ((right_half + left_half) & (MAX - 1)); //(unsigned int) (x | y);
    }
    big_integer ans = big_integer(tmp);
    ans.sign = sign;
    *this = ans;

    if (sign == -1) {
        sign = 1;
        two_complainment(data.size());
        sign = -1;
    }

    remove_extra_zeros();
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    if (rhs == 0) {
        return *this;
    }

    unsigned int full_cells = rhs >> 5;
    unsigned int left = rhs % base2;
    unsigned int newsize = data.size() - full_cells;

    if (sign == -1) {
        two_complainment(size());
    }

    if (newsize < 0) {
        *this = big_integer(0);
        return *this;
    }
    std::vector<unsigned int> tmp(newsize);
    for (size_t i = 0; i < newsize; i++) {
        unsigned long long right_half = (unsigned long long) (get(data, i + full_cells)) >> left;
        unsigned long long left_half = (unsigned long long) (get(data, i + full_cells + 1)) << (base2 - left);
        tmp[i] = (unsigned int) ((right_half + left_half) & (MAX - 1));
    }
    big_integer ans = big_integer(tmp);
    ans.sign = (ans.data.back() >> (base2 - 1)) == 1 ? -1 : 1;
    *this = ans;

    if (sign == -1) {
        sign = 1;
        two_complainment(size());
        sign = -1;
    }

    remove_extra_zeros();
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer tmp = big_integer(*this);
    tmp.sign = -tmp.sign;
    return tmp;
}

unsigned int big_integer::size() {
    return data.size();
}

big_integer big_integer::operator~() const {
    big_integer ans(*this);

    if (ans.sign == -1) {
        ans.two_complainment(ans.size());
    }

    unsigned int sz = ans.size();
    for (int i = 0; i < sz; i++) {
        ans.data[i] = (~ans.data[i]);
    }

    ans.sign = (ans.data.back() >> (base2 - 1)) == 1 ? -1 : 1;

    if (ans.sign == -1) {
        ans.sign = 1;
        ans.two_complainment(ans.size());
        ans.sign = -1;
    }

    return ans;
}

big_integer &big_integer::operator++() {
    *this += big_integer(1);
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer &big_integer::operator--() {
    *this -= big_integer(1);
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

int compare(big_integer const &first, big_integer const &second) {   // 0 - =, 2 - <, 1 - >
    if (first.is_zero() && second.is_zero()) {
        return 0;
    }
    if (first.sign != second.sign) {
        if (first.sign == 1) {
            return 1;
        } else {
            return 2;
        }
    }
    if (first.data.size() != second.data.size()) {
        if (first.data.size() > second.data.size()) {
            return 1;
        } else {
            return 2;
        }
    }

    for (int i = first.data.size() - 1; i > -1; i--) {
        if (first.data[i] > second.data[i]) {
            return 1;
        } else if (first.data[i] < second.data[i]) {
            return 2;
        }
    }
    return 0;
}

bool operator==(big_integer const &a, big_integer const &b) {
    int cmp = compare(a, b);
    return !cmp;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    int cmp = compare(a, b);
    return cmp != 0;
}

bool operator<(big_integer const &a, big_integer const &b) {
    int cmp = compare(a, b);
    return cmp == 2;
}

bool operator>(big_integer const &a, big_integer const &b) {
    int cmp = compare(a, b);
    return cmp == 1;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    int cmp = compare(a, b);
    return cmp == 2 || cmp == 0;
}

bool operator>=(big_integer const &a, big_integer const &b) {
    int cmp = compare(a, b);
    return cmp == 1 || cmp == 0;
}

std::string to_string(big_integer const &a) {
    std::string ans = "";
    big_integer tmp(a);
    if (tmp.is_zero()) {
        return "0";
    }
    std::string sign;
    if (tmp.sign == -1) {
        sign = "-";
        tmp.sign = 1;
    }
    while (tmp > 0) {
        ans = (char) ((tmp % 10).data.back() + '0') + ans;
        tmp /= 10;
    }
    return sign + ans;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}
