#include<iostream>
#include<fstream>
#include<cmath>
#include <cctype>

template<class T>
class Stack final {
private:
    struct Node final {
        T val;
        Node* next;
        Node() : val(T()), next(nullptr) {}
        Node(const T& el) : val(el), next(nullptr) {}
        Node(T&& el) : val(std::move(el)), next(nullptr) {}
        Node(const Node& other) = delete;
        Node& operator=(const Node& other) = delete;
    };
    Node* top;
    size_t sz;
public:
    Stack() : top(nullptr), sz(0) {}
    Stack(const Stack& other) : top(nullptr), sz(0) {
        if (other.empty()) return;
        top = new Node(other.top->val);
        Node* current = top;
        Node* otherCurrent = other.top->next;
        sz = 1;
        while (otherCurrent) {
            current->next = new Node(otherCurrent->val);
            current = current->next;
            otherCurrent = otherCurrent->next;
            ++sz;
        }
    }
    Stack(Stack&& other) noexcept : top(other.top), sz(other.sz) {
        other.top = nullptr;
        other.sz = 0;
    }
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            Stack temp(other);
            swap(temp);
        }
        return *this;
    }
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            top = other.top;
            sz = other.sz;
            other.top = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    ~Stack() noexcept {
        clear();
    }
    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        ++sz;
    }
    void push(T&& value) {
        Node* newNode = new Node(std::move(value));
        newNode->next = top;
        top = newNode;
        ++sz;
    }
    template<typename... Args>
    void emplace(Args&&... args) {
        Node* newNode = new Node(T(std::forward<Args>(args)...));
        newNode->next = top;
        top = newNode;
        ++sz;
    }
    void pop() noexcept {
        Node* oldTop = top;
        top = top->next;
        delete oldTop;
        --sz;
    }
    T& back() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return top->val;
    }
    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return top->val;
    }
    bool empty() const noexcept {
        return top == nullptr;
    }
    size_t size() const noexcept {
        return sz;
    }
    void clear() noexcept {
        while (!empty()) {
            pop();
        }
    }
    void swap(Stack& other) noexcept {
        std::swap(top, other.top);
        std::swap(sz, other.sz);
    }
    bool operator==(const Stack& other) const {
        if (sz != other.sz) {
            return false;
        }
        Node* current1 = top;
        Node* current2 = other.top;
        while (current1 && current2) {
            if (current1->val != current2->val) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }
        return current1 == nullptr && current2 == nullptr;
    }
    bool operator!=(const Stack& other) const {
        return !(*this == other);
    }
};

template<typename T>
class DynamicArray {
private:
    T* data_ptr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
public:
    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
    private:
        pointer m_ptr;
    public:
        iterator(pointer ptr = nullptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }
        iterator& operator++() { ++m_ptr; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        iterator& operator--() { --m_ptr; return *this; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }
        iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        iterator operator+(difference_type n) const { iterator tmp = *this; tmp += n; return tmp; }
        iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }
        iterator operator-(difference_type n) const { iterator tmp = *this; tmp -= n; return tmp; }
        difference_type operator-(const iterator& other) const { return m_ptr - other.m_ptr; }
        reference operator[](difference_type n) const { return *(m_ptr + n); }
        bool operator==(const iterator& other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        bool operator<(const iterator& other) const { return m_ptr < other.m_ptr; }
        bool operator>(const iterator& other) const { return other < *this; }
        bool operator<=(const iterator& other) const { return !(*this > other); }
        bool operator>=(const iterator& other) const { return !(*this < other); }
    };
    class const_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
    private:
        pointer m_ptr;
    public:
        const_iterator(pointer ptr = nullptr) : m_ptr(ptr) {}
        const_iterator(const iterator& it) : m_ptr(it.m_ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }
        const_iterator& operator++() { ++m_ptr; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        const_iterator& operator--() { --m_ptr; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }
        const_iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        const_iterator operator+(difference_type n) const { const_iterator tmp = *this; tmp += n; return tmp; }
        const_iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }
        const_iterator operator-(difference_type n) const { const_iterator tmp = *this; tmp -= n; return tmp; }
        difference_type operator-(const const_iterator& other) const { return m_ptr - other.m_ptr; }
        reference operator[](difference_type n) const { return *(m_ptr + n); }
        bool operator==(const const_iterator& other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
        bool operator<(const const_iterator& other) const { return m_ptr < other.m_ptr; }
        bool operator>(const const_iterator& other) const { return other < *this; }
        bool operator<=(const const_iterator& other) const { return !(*this > other); }
        bool operator>=(const const_iterator& other) const { return !(*this < other); }
    };
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    DynamicArray() {}
    DynamicArray(size_t sz) {
        resize(sz);
    }
    DynamicArray(size_t sz, T default_val) {
        resize(sz);
        for (size_t i = 0; i < sz; ++i) {
            data_ptr[i] = default_val;
        }
    }
    DynamicArray(const std::initializer_list<T>& params) {
        if (params.size() > 0) {
            resize(params.size());
            size_t i = 0;
            for (auto elem : params) {
                data_ptr[i] = std::move(elem);
                ++i;
            }
        }
    }
    DynamicArray(const DynamicArray& other) {
        if (this != &other) {
            resize(other.sz);
            for (size_t i = 0; i < sz; ++i) {
                data_ptr[i] = other.data_ptr[i];
            }
        }
    }
    DynamicArray(DynamicArray&& other) noexcept {
        if (this != &other) {
            data_ptr = other.data_ptr;
            sz = other.sz;
            cap = other.cap;
            other.data_ptr = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
    }
    ~DynamicArray() noexcept {
        sz = cap = 0;
        delete[] data_ptr;
        data_ptr = nullptr;
    }
    T& operator[](size_t ind) {
        return data_ptr[ind];
    }
    const T operator[](size_t ind) const {
        return data_ptr[ind];
    }
    void reserve(size_t new_cap) {
    if (new_cap <= cap) return;
        auto new_mem = new T[new_cap];
        for (size_t i = 0; i < sz; ++i) {
            new_mem[i] = std::move(data_ptr[i]);
        }
        delete[] data_ptr;
        data_ptr = new_mem;
        cap = new_cap;
    }
    void resize(size_t new_sz) {
        if (new_sz < sz) {
            for (size_t i = new_sz; i < sz; ++i) data_ptr[i].~T();
        }
        if (new_sz > cap) {
            reserve(std::max(new_sz, cap * 2));
        }
        if (new_sz > sz) {
            for (size_t i = sz; i < new_sz; ++i) {
                new (&data_ptr[i]) T();
            }
        }
        sz = new_sz;
    }
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data_ptr;
            data_ptr = new T[other.cap];
            sz = other.sz;
            cap = other.cap;
            for (size_t i = 0; i < sz; ++i) {
                data_ptr[i] = other.data_ptr[i];
            }
        }
        return *this;
    }
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            data_ptr = other.data_ptr;
            sz = other.sz;
            cap = other.cap;
            other.data_ptr = nullptr;
            other.sz = 0;
            other.cap = 0;
        }

        return *this;
    }
    size_t size() const {
        return sz;
    }
    template<typename U>
    void push_back(U&& val) {
        resize(sz + 1);
        data_ptr[sz - 1] = std::forward<U>(val);
    }
    T pop_back() {
        --sz;
        return data_ptr[sz];
    }
    template<typename U>
    void insert(size_t pos, U&& val) {
        resize(sz + 1);
        for (size_t i = sz - 1; i > pos; --i) {
            data_ptr[i] = std::move(data_ptr[i - 1]);
        }
        data_ptr[pos] = std::forward<U>(val);
    }
    void remove(size_t pos) {
        for (size_t i = pos; i < sz - 1; ++i) {
            data_ptr[i] = std::move(data_ptr[i + 1]);;
        }
        resize(sz - 1);
    }
    void clear() noexcept {
        for (size_t i = 0; i < sz; ++i) {
            data_ptr[i] = T{};
        }
        sz = 0;
    }
    iterator begin() noexcept { return iterator(data_ptr); }
    iterator end() noexcept { return iterator(data_ptr + sz); }
    const_iterator begin() const noexcept { return const_iterator(data_ptr); }
    const_iterator end() const noexcept { return const_iterator(data_ptr + sz); }
    const_iterator cbegin() const noexcept { return const_iterator(data_ptr); }
    const_iterator cend() const noexcept { return const_iterator(data_ptr + sz); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
};

template<class T, class U, class Comp = std::less<T>>
class AvlTree final {
private:
    using schar = signed char;
    struct Node final {
        T key;
        U val;
        Node* parent{};
        Node* left{};
        Node* right{};
        schar balance{0};
        Node(const T& key_, const U& val_) : key(key_), val(val_), balance(0) {}
        Node() : key(T()), val(U()), balance(0) {}
        Node(T&& key_, U&& val_) : key(std::move(key_)), val(std::move(val_)), balance(0) {}
    };
    enum comprassion {
        less, equals, greater
    };
    using nodeptr = Node*;
    Comp comp;
    nodeptr root;
    size_t sz;
    inline bool not_root(nodeptr node) const noexcept {
        return node != nullptr && node->parent != nullptr;
    }
    inline bool is_left(nodeptr node) const noexcept {
        return not_root(node) && node->parent->left == node;
    }
    inline bool is_right(nodeptr node) const noexcept {
        return not_root(node) && node->parent->right == node;
    }
    inline bool has_right_brother(nodeptr node) const noexcept {
        return is_left(node) && node->parent->right;
    }
    inline bool has_left_brother(nodeptr node) const noexcept {
        return is_right(node) && node->parent->left;
    }
    inline nodeptr rotate_right(nodeptr node) noexcept {
        if (!node || !node->left) {
            return node;
        }
        nodeptr new_node = node->left;
        node->left = new_node->right;
        if (new_node->right) {
            new_node->right->parent = node;
        }
        new_node->parent = node->parent;
        if (!node->parent) {
            root = new_node;
        } else if (node->parent->left == node) {
            node->parent->left = new_node;
        } else {
            node->parent->right = new_node;
        }
        new_node->right = node;
        node->parent = new_node;
        return new_node;
    }
    inline nodeptr rotate_left(nodeptr node) noexcept {
        if (!node || !node->right) {
            return node;
        }
        nodeptr new_node = node->right;
        node->right = new_node->left;
        if (new_node->left) {
            new_node->left->parent = node;
        }
        new_node->parent = node->parent;
        if (!node->parent) {
            root = new_node;
        } else if (node->parent->left == node) {
            node->parent->left = new_node;
        } else {
            node->parent->right = new_node;
        }
        new_node->left = node;
        node->parent = new_node;
        return new_node;
    }
    inline nodeptr find_left(nodeptr node) const noexcept {
        if (!node) {
            return node;
        }
        auto now{node};
        while (now->left) {
            now = now->left;
        }
        return now;
    }
    inline nodeptr find_right(nodeptr node) const noexcept {
        if (!node) {
            return node;
        }
        auto now{node};
        while (now->right) {
            now = now->right;
        }
        return now;
    }
    inline nodeptr find(nodeptr node, const T& key) const noexcept {
        if (!node) {
            return node;
        }
        auto now = node;
        while (now) {
            if (!comp(now->key, key) && !comp(key, now->key)) {
                break;
            }
            now = (comp(key, now->key) ? now->left : now->right);
        }
        return now;
    }
    inline std::pair<nodeptr, comprassion> find_par(nodeptr node, const T& key) const noexcept {
        nodeptr now = node;
        comprassion cmp;
        nodeptr par = node->parent;
        while (now) {
            if (!comp(key, now->key) && !comp(now->key, key)) {
                return {now, equals};
            }
            par = now;
            if (comp(key, now->key)) {
                now = now->left;
                cmp = less;
            } else {
                now = now->right;
                cmp = greater;
            }
        }
        return {par, cmp};
    }
    inline nodeptr preorderInit(const nodeptr node) const noexcept {
        if (!node) {
            return nullptr;
        }
        auto ptr{node};
        while (ptr->left || ptr->right) {
            if (ptr->left) {
                ptr = ptr->left;
            } else {
                ptr = ptr->right;
            }
        }
        return ptr;
    }
    inline nodeptr InorderInit(nodeptr node) const noexcept {
        if (!node) {
            return nullptr;
        }
        while (node->left) {
            node = node->left;
        }
        return node;
    }
    nodeptr InorderNext(nodeptr cur) const noexcept {
        if (!cur) {
            return nullptr;
        }
        if (cur->right) {
            cur = cur->right;
            while (cur->left) {
                cur = cur->left;
            }
            return cur;
        }
        nodeptr par = cur->parent;
        while (par && cur == par->right) {
            cur = par;
            par = par->parent;
        }
        return par;
    }
    const nodeptr preorderNext(const nodeptr node) const noexcept {
        return (has_right_brother(node) ? preorderInit(node->parent->right) : node->parent);
    }
    inline nodeptr bal(nodeptr& node) {
        if (!node) {
            return nullptr;
        }
        if (node->balance == 2) {
            nodeptr B = node->left;
            if (B->balance >= 0) {
                // LL
                nodeptr new_root = rotate_right(node);
                if (B->balance == 0) {
                    node->balance = 1;
                    B->balance = -1;
                } else {
                    node->balance = 0;
                    B->balance = 0;
                }
                return new_root;
            } else {
                // LR
                nodeptr C = B->right;
                rotate_left(B);
                nodeptr new_root = rotate_right(node);
                if (C->balance == 1) {
                    node->balance = -1;
                    B->balance = 0;
                } else if (C->balance == 0) {
                    node->balance = 0;
                    B->balance = 0;
                } else {
                    node->balance = 0;
                    B->balance = 1;
                }
                C->balance = 0;
                return new_root;
            }
        } else if (node->balance == -2) {
            nodeptr B = node->right;
            if (B->balance <= 0) {
                // RR
                nodeptr new_root = rotate_left(node);
                if (B->balance == 0) {
                    node->balance = -1;
                    B->balance = 1;
                } else {
                    node->balance = 0;
                    B->balance = 0;
                }
                return new_root;
            } else { 
                // RL
                nodeptr C = B->left;
                rotate_right(B);
                nodeptr new_root = rotate_left(node);
                if (C->balance == -1) {
                    node->balance = 1;
                    B->balance = 0;
                } else if (C->balance == 0) {
                    node->balance = 0;
                    B->balance = 0;
                } else {
                    node->balance = 0;
                    B->balance = -1;
                }
                C->balance = 0;
                return new_root;
            }
        }
        return node;
    }
    inline void balance_add(nodeptr now) noexcept {
        while (now && now->parent) {
            nodeptr p = now->parent;
            if (comp(now->key, p->key)) {
                ++p->balance;
            } else {
                --p->balance;
            }
            if (!p->balance) {
                break;
            }
            if (p->balance == 2 || p->balance == -2) {
                bal(p);
                break;
            }
            now = p;
        }
    }
    inline bool add(nodeptr& node, const T& key, const U& val) noexcept {
        if (!node) {
            node = new Node(key, val);
            ++sz;
            return true;
        }
        auto [pos, cmp] = find_par(node, key);
        if (cmp == equals) {
            return false; 
        }
        nodeptr new_node = new Node(key, val);
        new_node->parent = pos;
        if (cmp == less) {
            pos->left = new_node;
        } else {
            pos->right = new_node;
        }
        ++sz;
        balance_add(new_node);
        return true;
    }
    inline void balance_del(nodeptr node) {
        while (node) {
            auto parent = node->parent;
            bool next_was_left = (parent && parent->left == node);
            auto new_node = bal(node);
            if (new_node->balance) {
                break;
            }
            node = parent;
            if (node) {
                if (next_was_left) {
                    --node->balance;
                } else {
                    ++node->balance;
                }
            }
        }
    }
    inline bool del(nodeptr& node, const T& key) {
        nodeptr now = find(node, key);
        if (!now) {
            return false;
        }
        auto del_elem = (now->left && now->right) ? find_right(now->left) : now;
        if (del_elem != now) {
            now->key = std::move(del_elem->key);
            now->val = std::move(del_elem->val);
        }
        nodeptr child = del_elem->left ? del_elem->left : del_elem->right;
        nodeptr parent = del_elem->parent;
        bool was_left = is_left(del_elem);
        if (child) {
            child->parent = parent;
        }
        if (!parent) {
            root = child;
        } else if (was_left) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        delete del_elem;
        --sz;
        if (parent) {
            if (was_left) {
                --parent->balance;
            } else {
                ++parent->balance;
            }
            balance_del(parent);
        }
        return true;
    }
    inline void clear(nodeptr& node) noexcept {
        auto now{preorderInit(node)};
        while (now) {
            const auto nxt{preorderNext(now)};
            delete now;
            now = nxt;
        }
    }
    void debug_print(nodeptr node, size_t lev = 0) const noexcept {
        if (!node) return;
        std::cout << "{" << node->key << ' ' << node->val << "} " << "{" << (int)node->balance << ' ' << lev << "}\n";
        debug_print(node->left);
        debug_print(node->right);
    }
    inline void serialize_rec(std::ofstream& os, const nodeptr& nd) const noexcept {
        os.write((char*)&sz, sizeof(size_t));
        nodeptr cur = InorderInit(nd);
        while (cur) {
            size_t len = cur->key.size();
            os.write((char*)&len, sizeof(size_t));
            os.write(cur->key.c_str(), len);
            os.write((char*)&cur->val, sizeof(U));
            cur = InorderNext(cur);
        }
    }
    inline nodeptr deserialize_rec(std::ifstream& is) {
        DynamicArray<std::pair<T, U>> vec;
        size_t size;
        is.read((char*)&size, sizeof(size_t));
        if (!size) {
            return nullptr;
        }
        char* buf = new char[256];
        vec.resize(size);
        for (size_t i = 0; i < vec.size(); ++i) {
            size_t len;
            is.read((char*)&len, sizeof(size_t));
            is.read(buf, len);
            T key(buf, len);
            U val;
            is.read((char*)&val, sizeof(U));
            vec[i] = {std::move(key), val};
        }
        delete[] buf;
        struct Item {
            nodeptr par;
            long long l, r;
            bool fl;
        };
        Stack<Item> st;
        nodeptr node = nullptr;
        st.push({node, 0, vec.size() - 1, 0});
        while (!st.empty()) {
            auto [par, l, r, fl] = st.back();
            st.pop();
            if (l > r) {
                continue;
            }
            size_t m = l + (r - l) / 2;
            //std::cerr << l << ' ' << m << ' ' << r << '\n';
            auto now = new Node(std::move(vec[m].first), std::move(vec[m].second));
            now->parent = par;
            if (!par) {
                node = now;
            } else if (fl) {
                par->right = now;
            } else {
                par->left = now;
            }
            size_t lsz = (m > l) ? (m - l) : 0;
            size_t rsz = (r > m) ? (r - m) : 0;
            now->balance = static_cast<schar>(lsz > rsz ? 1 : (lsz < rsz ? -1 : 0));
            if (m + 1 <= r) {
               st.push({now, m + 1, r, 1});
            }
            if (l <= m - 1) {
                st.push({now, l, m - 1, 0}); 
            }
        }
        return node;
    }
    inline nodeptr copy(nodeptr other) {
        if (!other) {
            return nullptr;
        }
        auto node = new Node(other->key, other->val);
        node->balance = other->balance;
        node->parent = nullptr;
        auto ptr = other;
        auto copy = node;
        nodeptr par = nullptr;
        while (ptr) {
            if (par == ptr->parent) {
                if (ptr->left) {
                    copy->left = new Node(ptr->left->key, ptr->left->val);
                    copy->left->balance = ptr->left->balance;
                    copy->left->parent = copy;
                    par = ptr;
                    ptr = ptr->left;
                    copy = copy->left;
                    continue;
                } else if (ptr->right) {
                    copy->right = new Node(ptr->right->key, ptr->right->val);
                    copy->right->balance = ptr->right->balance;
                    copy->right->parent = copy;
                    par = ptr;
                    ptr = ptr->right;
                    copy = copy->right;
                    continue;
                }
            } else if (par == ptr->left) {
                if (ptr->right) {
                    copy->right = new Node(ptr->right->key, ptr->right->val);
                    copy->right->balance = ptr->right->balance;
                    copy->right->parent = copy;
                    par = ptr;
                    ptr = ptr->right;
                    copy = copy->right;
                    continue;
                }
            }
            par = ptr;
            ptr = ptr->parent;
            if (copy) {
                copy = copy->parent;
            }
        }
        return node;
    }
public:
    AvlTree() : sz(0), root(nullptr) {}
    AvlTree(const AvlTree& other) {
        sz = other.sz;
        root = copy(other.root);
    }
    AvlTree& operator=(const AvlTree& other) {
        sz = other.sz;
        root = copy(other.root);
        return *this;
    }
    AvlTree(AvlTree&& other) : sz(other.sz), root(nullptr) {
        std::swap(root, other.root);
        sz = other.sz;
        other.sz = 0;
    }
    AvlTree& operator=(AvlTree&& other) noexcept {
        this->sz = other.sz;
        this->root = nullptr;
        std::swap(this->root, other.root);
        other.sz = 0;
        return *this;
    }
    AvlTree(const Comp& cmp) {
        comp = cmp;
    }
    ~AvlTree() noexcept {
        clear(root);
    }
    size_t size() const {
        return sz;
    }
    void serialize(std::ofstream& os) const {
        serialize_rec(os, root);
    }
    void deserialize(std::ifstream& is) {
        root = deserialize_rec(is);
    }
    nodeptr find(const T& key) const {
        return find(root, key);
    }
    bool contains(const T& key) const {
        return find(root, key) != nullptr;
    }
    bool del(const T& key) {
        return del(root, key);
    }
    bool add(const T& key, const U& val) noexcept {
        return add(root, key, val);
    }
    bool add(T&& key, U&& val) noexcept {
        return add(root, std::move(key), std::move(val));
    }
    void debug_print() {
        debug_print(root);
    }
};

class string {
private:
    char* str;
    size_t len;
public:
    string() : str(nullptr), len(0) {
        str = new char[1];
        str[0] = '\0';
    }
    string(const char* cstr) : len(0) {
        if (cstr) {
            while (cstr[len] != '\0') {
                ++len;
            }
            str = new char[len + 1];
            for (size_t i = 0; i < len; ++i) {
                str[i] = cstr[i];
            }
            str[len] = '\0';
        } else {
            str = new char[1];
            str[0] = '\0';
            len = 0;
        }
    }
    string(const char* cstr, size_t length) : len(length) {
        if (length == 0 || cstr == nullptr) {
            str = new char[1];
            str[0] = '\0';
            len = 0;
            return;
        }
        str = new char[len + 1];
        for (size_t i = 0; i < len; ++i) {
            str[i] = cstr[i];
        }
        str[len] = '\0';
    }
    string(const string& other) : len(other.len)  {
        str = new char[len + 1];
        for (size_t i = 0; i < len; ++i) {
            str[i] = other.str[i];
        }
        str[len] = '\0';
    }
    string& operator=(const string& other) {
        if (this != &other) {
            delete[] str;
            len = other.len;
            str = new char[len + 1];
            for (size_t i = 0; i < len; ++i) {
                str[i] = other.str[i];
            }
            str[len] = '\0';
        }
        return *this;
    }
    string(string&& other) noexcept : str(other.str), len(other.len) {
        other.str = nullptr;
        other.len = 0;
    }
    string& operator=(string&& other) noexcept {
        if (this != &other) {
            delete[] str;
            str = other.str;
            len = other.len;
            other.str = nullptr;
            other.len = 0;
        }
        return *this;
    }
    const char* c_str() const noexcept {
        return str;
    }
    size_t size() const noexcept {
        return len;
    }
    bool empty() const noexcept {
        return len == 0;
    }
    void clear() noexcept {
        delete[] str;
        str = new char[1];
        str[0] = '\0';
        len = 0;
    }
    char& operator[](size_t ind) {
        return str[ind];
    }
    const char& operator[](size_t ind) const {
        return str[ind];
    }
    friend std::istream& operator>>(std::istream& is, string& s) {
        char buf[257];
        is >> buf;
        s = string(buf);
        return is;
    }
    bool operator==(const string& other) const {
        if (len != other.len) {
            return false;
        }
        for (size_t i = 0; i < len; ++i) {
            if (tolower(str[i]) != tolower(other.str[i])) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const string& other) const {
        return !(*this == other);
    }
    bool operator<(const string& other) const {
        size_t min_len = (len < other.len) ? len : other.len;
        for (size_t i = 0; i < min_len; ++i) {
            if (tolower(str[i]) != tolower(other.str[i])) {
                return tolower(str[i]) < tolower(other.str[i]);
            }
        }
        return len < other.len;
    }
    bool operator<=(const string& other) const {
        return (*this < other) || (*this == other);
    }
    bool operator>(const string& other) const {
        return !(*this <= other);
    }
    bool operator>=(const string& other) const {
        return !(*this < other);
    }
    friend std::ostream& operator<<(std::ostream& os, const string& s) {
        for (size_t i = 0; i < s.size(); ++i) {
            os << s[i];
        }
        return os;
    }  
    ~string() noexcept {
        delete[] str;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
    AvlTree<string, size_t> mp;
    string s;
    while (std::cin >> s) {
        if (s == "+") {
            string key;
            std::cin >> key;
            size_t val;
            std::cin >> val;
            std::cout << (mp.add(std::move(key), std::move(val)) ? "OK\n" : "Exist\n");
        } else if (s == "-") {
            string key;
            std::cin >> key;
            std::cout << (mp.del(key) ? "OK\n" : "NoSuchWord\n");
        } else if (s == "!") {
            string command;
            std::cin >> command;
            if (command == "Save") {
                string file_name;
                std::cin >> file_name;
                std::ofstream os(file_name.c_str(), std::ios::binary);
                mp.serialize(os);
            } else if (command == "Load") {
                string file_name;
                std::cin >> file_name;
                std::ifstream is(file_name.c_str(), std::ios::binary);
                mp.deserialize(is);
            }
            std::cout << "OK\n";
        } else if (s == "Print") {
            mp.debug_print();
        } else if (s == "Copy") {
            auto new_tree = mp;
            new_tree.debug_print();
        } else {
            auto it = mp.find(s);
            if (it) {
                std::cout << "OK: " << it->val << '\n';
            } else {
                std::cout << "NoSuchWord\n";
            }
        }
    }
    return 0;
}