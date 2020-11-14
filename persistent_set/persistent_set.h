#ifndef PERSISTENT_SET_PERSISTENT_SET_H
#define PERSISTENT_SET_PERSISTENT_SET_H

//TODO: fix begin(), end()              vrode norm
//TODO: test memory leaks
//TODO: test iterators, aka ++/--       vrode norm
//TODO: ask someone about uslovie
//TODO: all?

#include <memory>
#include <vector>
#include <map>

template<typename T>
struct persistent_set {
    // Вы можете определить этот тайпдеф по вашему усмотрению.
//private:
    struct Node;
//public:


    // Bidirectional iterator.
    struct iterator : std::iterator<std::bidirectional_iterator_tag, const T> {

        iterator(): node(nullptr), root(nullptr), is_end(false){}

        T const &operator*() const {
            return node->val;
        }

        T const * operator->() const{
            return &node->val;
        }

        explicit iterator(Node *nd, Node *root) : node(nd), root(root), is_end(false) {}

        iterator(bool end_, Node *root) : node(nullptr), root(root), is_end(false) {
            if (end_) {
                is_end = true;
            }
        }

        bool operator==(iterator const &other) const {
            return (node == other.node) || (is_end && other.is_end);
        }

        iterator &operator++() {
            if(node == nullptr || root == nullptr) {
                *this = iterator(true, nullptr);
                return *this;
            }

            Node *max = root;
            while(max->right != nullptr)
                max = max->right.get();

            if(this->node == max){
                *this = iterator(true, root);
                return(*this);
            }

            T x = *(*this);

            Node *current = root, *succesor = nullptr;

            while (current != nullptr) {
                if (current->val > x) {
                    succesor = current;
                    current = current->left.get();
                } else {
                    current = current->right.get();
                }
            }

            *this = iterator(succesor, root);
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator &operator--() {
            if (is_end) {
                Node *current = root;
                while (current->right != nullptr)
                    current = current->right.get();

                *this = iterator(current, root);

                return *this;
            }

            T x = *(*this);

            Node *current = root, *precessor = nullptr;

            while (current != nullptr) {
                if (current->val < x) {
                    precessor = current;
                    current = current->right.get();
                } else {
                    current = current->left.get();
                }
            }

            *this = iterator(precessor, root);
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator!=(iterator const &other) const {
            return !(*this == other);
        }

    private:
        Node *node;
        Node *root;
        bool is_end;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;

    using const_iterator = iterator;

    Node* get_root() const{
        if(is_empty())
            return nullptr;
        return roots.back().get();
    }

    reverse_iterator rbegin() const {

        return reverse_iterator(end());
    }

    reverse_iterator rend() const {
        return reverse_iterator(begin());
    }

    // Создает пустой persistent_set.
    persistent_set() : roots(0) {}

    bool is_empty() const {
        return roots.size() == 0;
    }

    bool empty(){
        return is_current_version_empty();
    }

    void clear(){
        if(!is_empty()) {
            roots.back() = nullptr;
        }
    }

    bool is_current_version_empty() {
        if(roots.empty())
            return true;
        return roots.back().get() == nullptr;
    }

    void swap(persistent_set& other){
        std::swap(roots, other.roots);
    }

    // Создает копию указанного persistent_set-а.
    persistent_set(persistent_set const &other) {
        if(!other.roots.empty()){
            roots = std::vector<std::shared_ptr<Node>>{other.roots.back()};
        }
    }

    // Изменяет this так, чтобы он содержал те же элементы, что и rhs.
    // Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    persistent_set &operator=(persistent_set const &rhs) {
        persistent_set<T> tmp(rhs);
        swap(tmp);
        return *this;
    }

    // Деструктор. Вызывается при удалении объектов persistent_set.
    // Инвалидирует все итераторы ссылающиеся на элементы этого persistent_set
    // (включая итераторы ссылающиеся на элемент следующий за последним).
    ~persistent_set() {}

    // Поиск элемента.
    // Возвращает итератор на найденный элемент, либо end(), если элемент
    // с указанным значением отсутвует.
    iterator find(T value) {
        if (is_empty()) {
            return end();
        }

        Node *curr = roots.back().get();

        while (curr != nullptr) {
            if (value == curr->val) {
                return iterator(curr, roots.back().get());
            }
            if (value > curr->val) {
                curr = curr->right.get();
            } else {
                curr = curr->left.get();
            }
        }

        return end();
    }

    // Вставка элемента.
    // 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
    //    на уже присутствующий элемент и false.
    // 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
    //    элемент и true.
    // Если вставка произведена, инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    std::pair<iterator, bool> insert(T value) {
        if (is_empty()) {
            roots.push_back(std::shared_ptr<Node>(new Node(value)));
            return std::make_pair(iterator(roots.back().get(), roots.back().get()), true);
        }

        iterator it = find(value);
        if (it != end()) {
            return std::make_pair(it, false);
        }

        Node *input = roots.back().get();
        Node *output = new Node(roots.back().get());

        roots.push_back(std::shared_ptr<Node>(output));

        while (input != nullptr) {
            if (value > input->val) {
                output->left = input->left;

                if (input->right != nullptr) {
                    output->right = std::shared_ptr<Node>(new Node(input->right.get()));
                } else {
                    output->right = std::shared_ptr<Node>(new Node(value));
                }

                input = input->right.get();
                output = output->right.get();
            } else {
                output->right = input->right;

                if (input->left != nullptr) {
                    output->left = std::shared_ptr<Node>(new Node(input->left.get()));
                } else {
                    output->left = std::shared_ptr<Node>(new Node(value));
                }

                input = input->left.get();
                output = output->left.get();
            }
        }

        return std::make_pair(iterator(output, roots.back().get()), false);
    };

    // Удаление элемента.
    // Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    void erase(iterator it) {
        if(is_empty())
            return;

        if(it == end())
            return;
        if (it == end()) {
            roots.push_back(roots.back());
            return;
        }
        if (find(*it) == end()) {
            roots.push_back(roots.back());
            return;
        }
//        if (*it == roots.back()->val && roots.back()->left == nullptr && roots.back()->right == nullptr) {
//            roots.push_back(nullptr);
//            return;
//        }

        Node *parent = roots.back().get();
        Node *son = parent;

        Node *output_parent = new Node(roots.back().get()->val);
        Node *output_son = output_parent;

        roots.push_back(std::shared_ptr<Node>(output_parent));

        while (son->val != *it) {
            parent = son;
            output_parent = output_son;


            if (parent->val < *it) {
                son = parent->right.get();
                output_son = new Node(parent->right.get()->val);
                output_parent->right = std::shared_ptr<Node>(output_son);
                output_parent->left = parent->left;
            } else {
                son = parent->left.get();
                output_son = new Node(parent->left.get()->val);
                output_parent->left = std::shared_ptr<Node>(output_son);
                output_parent->right = parent->right;
            }
        }

        if (son->left != nullptr) {
            Node *max_parent = son;
            Node *max = son->left.get();

            while (max->right != nullptr) {
                max_parent = max;
                max = max->right.get();
            }

            Node *output;

            if (parent->left.get() == son) {
                if (son != parent) {
                    output_parent->left = std::shared_ptr<Node>(new Node(max));
                    output = output_parent->left.get();
                } else {
                    roots.back() = std::shared_ptr<Node>(new Node(max));
                    output = roots.back().get();
                }
            } else {
                if (son != parent) {
                    output_parent->right = std::shared_ptr<Node>(new Node(max));
                    output = output_parent->right.get();
                } else {
                    Node* tmp = new Node(max);
                    tmp->right = son->right;
//                    tmp->left = max->right;
                    roots.back() = std::shared_ptr<Node>(tmp);
                    output = roots.back().get();
                }
            }

            Node *input = son->left.get();

            if (input == max) {
                output->left = max->left;
            } else {
                output->left = std::shared_ptr<Node>(new Node(input->val));
                output->right = son->right;
                output = output->left.get();

                while (input != max) {
                    if (input->right.get() != max) {
                        output->right = std::shared_ptr<Node>(new Node(input->right.get()));
                    } else {
                        output->right = max->left;
                    }
                    output->left = input->left;

                    input = input->right.get();
                    output = output->right.get();
                }
            }
        } else {
            if (son != parent) {
                if (output_son == output_parent->left.get()) {
                    output_parent->left = son->right;
                } else {
                    output_parent->right = son->right;
                }
            } else {
                roots.back() = son->right;
            }
        }
    }

    // Возващает итератор на элемент с минимальный ключом.
    iterator begin() const {
        if(is_empty())
            return iterator(nullptr, nullptr);

        if(roots.back() == nullptr){
            return iterator(nullptr, nullptr);
        }

        Node *curr = roots.back().get();
        while (curr->left != nullptr) {
            curr = curr->left.get();
        }

        return iterator(curr, roots.back().get());
    }

    // Возващает итератор на элемент следующий за элементом с максимальным ключом.
    iterator end() const {
        if(is_empty())
            return iterator(true, nullptr);
        return iterator(true, roots.back().get());
    }

//    void del(Node *nd, std::set<Node *> &deleted);

    struct Node {
        T val;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node() : left(nullptr), right(nullptr) {}

        explicit Node(T val) : val(val), left(nullptr), right(nullptr) {}

        Node(Node const &other) : val(other.val), left(other.left), right(other.right) {}

        explicit Node(Node *other) : val(other->val), left(nullptr), right(nullptr) {}
    };

private:
    std::vector<std::shared_ptr<Node>> roots;
};

template <typename T>
void swap(persistent_set<T>& x, persistent_set<T>& y){
    x.swap(y);
}

#endif //PERSISTENT_SET_PERSISTENT_SET_H