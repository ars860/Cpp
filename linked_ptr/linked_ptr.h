#ifndef LINKED_PTR_LINKED_PTR_H
#define LINKED_PTR_LINKED_PTR_H

struct link {
    bool unique() {
        return (next == nullptr && prev == nullptr);
    }

    void remove_from_list() {
        if (prev != nullptr) {
            prev->next = this->next;
        }
        if (next != nullptr) {
            next->prev = this->prev;
        }
        next = nullptr;
        prev = nullptr;
    }

    void join(link *other) {
        this->prev = other;
        this->next = other->next;
        other->next = this;
    }

    void swap(link *other) {
        if (other->prev != nullptr) {
            other->prev->next = this;
        }
        if (other->next != nullptr) {
            other->next->prev = this;
        }

        if (next != nullptr) {
            next->prev = other;
        }
        if (prev != nullptr) {
            prev->next = other;
        }

        std::swap(prev, other->prev);
        std::swap(next, other->next);
    }

private:
    link *next = nullptr;
    link *prev = nullptr;
};

template<typename T>
class linked_ptr {
public:
    linked_ptr() : ptr(nullptr) {}

    explicit linked_ptr(T *t) {
        ptr = t;
    }

    template<typename U>
    explicit linked_ptr(U *u) {
        ptr = u;
    }

    template<typename U>
    void join(linked_ptr<U> const &other) {
        l.join(&other.l);

        this->ptr = other.ptr;
    }

    linked_ptr(linked_ptr &other) {
        join(other);
    }

    linked_ptr(linked_ptr const &other) {
        join(other);
    };

    template<typename U>
    linked_ptr(linked_ptr<U> const &other) {
        join(other);
    };

    T *get() const {
        return ptr;
    }

    T *operator->() const {
        return ptr;
    }

    T &operator*() const {
        return *ptr;
    }

    linked_ptr &operator=(linked_ptr const &other) {
        remove_from_list();
        this->join(other);
        return *this;
    }

    template<typename U>
    linked_ptr &operator=(linked_ptr<U> const &other) {
        remove_from_list();
        this->join(other);
        return *this;
    }

    bool unique() const {
        return l.unique() && (ptr != nullptr);
    }

    void swap(linked_ptr &other) {
        if (*this == other) {
            return;
        }
        l.swap(&other.l);
        std::swap(ptr, other.ptr);
    }

    friend void swap(linked_ptr &first, linked_ptr &second) {
        first.swap(second);
    }

    void reset(T *ptr) {
        remove_from_list();
        this->ptr = ptr;
    }

    void reset() {
        remove_from_list();
    }

    template<typename U>
    void reset(U *ptr) {
        remove_from_list();
        this->ptr = ptr;
    }

    ~linked_ptr() {
        remove_from_list();
    }

    bool operator==(T *t) const {
        return ptr == t;
    }

    bool operator!=(T *t) const {
        return ptr != t;
    }

    bool operator>(T *t) const {
        return ptr > t;
    }

    bool operator>=(T *t) const {
        return ptr >= t;
    }

    bool operator<(T *t) const {
        return ptr < t;
    }

    bool operator<=(T *t) const {
        return ptr <= t;
    }

    template<typename U>
    bool operator==(linked_ptr<U> const &other) const {
        return other.ptr == ptr;
    }

    template<typename U>
    bool operator!=(linked_ptr<U> const &other) const {
        return other.ptr != ptr;
    }

    template<typename U>
    bool operator<(linked_ptr<U> const &other) const {
        return other.ptr < ptr;
    }

    template<typename U>
    bool operator<=(linked_ptr<U> const &other) const {
        return other.ptr <= ptr;
    }

    template<typename U>
    bool operator>(linked_ptr<U> const &other) const {
        return other.ptr > ptr;
    }

    template<typename U>
    bool operator>=(linked_ptr<U> const &other) const {
        return other.ptr >= ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

private:
    template<typename U>
    friend
    class linked_ptr;

    mutable link l;
    T *ptr;

    void remove_from_list() {
        if (unique()) {
            if (ptr != nullptr) {
                delete ptr;
            }
        } else {
            l.remove_from_list();
        }
        ptr = nullptr;
    }
};

template<typename T>
bool operator==(T *t, const linked_ptr<T> &lk) {
    return t == lk.get();
}

template<typename T>
bool operator!=(T *t, const linked_ptr<T> &lk) {
    return t != lk.get();
}

template<typename T>
bool operator<(T *t, const linked_ptr<T> &lk) {
    return t < lk.get();
}

template<typename T>
bool operator<=(T *t, const linked_ptr<T> &lk) {
    return t <= lk.get();
}

template<typename T>
bool operator>(T *t, const linked_ptr<T> &lk) {
    return t > lk.get();
}

template<typename T>
bool operator>=(T *t, const linked_ptr<T> &lk) {
    return t >= lk.get();
}

template<typename T>
linked_ptr<T> make_linked(T *ptr) {
    return linked_ptr<T>(ptr);
}

#endif //LINKED_PTR_LINKED_PTR_H