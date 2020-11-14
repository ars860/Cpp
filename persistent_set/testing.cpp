#include <gtest/gtest.h>
#include "persistent_set.h"

TEST(correctness, insert_random) {
    persistent_set<int> x;
    std::set<int> used;
    int amount = 1000;

    for (int i = 0; i < amount; i++) {
        int r = 5;
        while (used.count(r)) {
            r = rand() % amount;
        }
        used.insert(r);

        x.insert(r);
    }

    std::set<int> res;
    for (auto i:x) {
        res.insert(i);
        std::cout << i << " ";
    }

    EXPECT_EQ(res, used);
}

TEST(correctness, insert_erase_random) {
    persistent_set<int> x;
    std::set<int> used;
//    srand(2);
    int amount = 1000;

    for (int i = 0; i < amount; i++) {
        int r = 5;
        while (used.count(r)) {
            r = rand() % amount;
        }
        used.insert(r);

        x.insert(r);
    }

    used.clear();
    for (int i = 0; i < amount; i++) {
        int r = 5;
        while (used.count(r)) {
            r = rand() % amount;
        }
        used.insert(r);

        x.erase(x.find(r));
    }

    EXPECT_EQ(x.is_current_version_empty(), true);
}

struct T {
    T() = delete;

    T(int val) : val(val) {}

    bool operator>(T const &other) {
        return val > other.val;
    }

    bool operator<(T const &other) {
        return val < other.val;
    }

    bool operator==(T const &other) {
        return val == other.val;
    }

    bool operator!=(T const &other) {
        return !((*this) == other);
    }

private:
    int val;
};

TEST(correctness, T_without_constructor) {
    persistent_set<T> x;
    x.insert(T(1));
    x.erase(x.find(T(1)));
    EXPECT_EQ(x.is_current_version_empty(), true);
}

TEST(correctness, copy_after_change) {
    persistent_set<int> x;
    int amount = 100;

    for (int i = 0; i < amount; i++) {
        x.insert(rand());
    }

    persistent_set<int> y(x);
    std::set<int> was_in_x;

    for (auto i:x)
        was_in_x.insert(i);

    for (int i = 0; i < amount; i++) {
        x.insert(rand());
    }

    std::set<int> now_in_y;
    for (auto i:y) {
        now_in_y.insert(i);
    }

    EXPECT_EQ(was_in_x, now_in_y);
}

TEST(correctness, operations_with_empty_set) {
    persistent_set<int> x;
    x.erase(x.begin());
    x.erase(x.end());

    std::set<int> empty_set;
    for (auto i:x) {
        empty_set.insert(i);
    }

    EXPECT_EQ(empty_set.size(), 0);
}
