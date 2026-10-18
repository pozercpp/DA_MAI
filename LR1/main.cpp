#include<iostream>
#include<limits>

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
    T& operator[](size_t idx) {
        return data_ptr[idx];
    }
    const T operator[](size_t idx) const {
        return data_ptr[idx];
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
    iterator upper_boud(iterator start, iterator end, const T& val) const noexcept {
        auto count = std::distance(start, end);
        while (count > 0) {
            auto step = count / 2;
            auto it = start + step;
            if (!(val < *it)) {
                start = ++it;
                count -= step + 1;
            } else {
                count = step;
            }
        }
        return start;
    }
    template<typename U>
    void add_sorted(U&& val) {
        auto pos = upper_boud(begin(), end(), val) - begin();
        insert(pos, std::forward<U>(val));
    }
};

struct elem {
    size_t key;
    DynamicArray<char> val;
    ~elem() noexcept = default;
    bool operator<(const elem& other) const {
        return key < other.key;
    }
    bool operator>(const elem& other) const {
        return key > other.key;
    }
    bool operator<=(const elem& other) const {
        return key <= other.key;
    }
    bool operator>=(const elem& other) const {
        return key >= other.key;
    }
    bool operator==(const elem& other) const {
        return key == other.key;
    }
    elem() : key(0), val() {}
    elem(size_t k, DynamicArray<char> v) : key(k), val(std::move(v)) {}
    elem(const elem& other) : key(other.key), val(other.val) {}
    elem(elem&& other) : key(other.key), val(std::move(other.val)) {}
    elem operator =(const elem& other) {
        key = other.key;
        val = other.val;
        return *this;
    }
    elem operator =(elem&& other) { 
        key = other.key;
        val = std::move(other.val);
        return *this;
    }
};

template<typename iterator>
size_t get_key(iterator it) {
    return it->key;
}

template<typename iterator>
void bucket_sort(iterator start, iterator end) {
	if (start == end) {
    	return;
    }
    auto min = get_key(start);
    auto max = get_key(start);
    size_t dist = static_cast<size_t>(std::distance(start, end));
    for (auto it = start; it != end; ++it) {
        if (min > get_key(it)) {
            min = get_key(it);
        }
        if (max < get_key(it)) {
            max = get_key(it);
        }
    }
    size_t range = max - min;
    auto get_ind = [&](size_t key) -> size_t {
        double d = static_cast<double>(key - min) / range;
        return static_cast<size_t>(static_cast<double>(dist - 1) * d);
    };
    DynamicArray<DynamicArray<elem>> buckets(dist);
    for (auto it = start; it != end; ++it) {
        auto& vec = buckets[get_ind(get_key(it))];
        vec.add_sorted(std::move(*it));
    }
    iterator ind = start;
    for (size_t i = 0; i < buckets.size(); ++i) {
        for (size_t j = 0; j < buckets[i].size(); ++j) {
            *ind = std::move(buckets[i][j]);
            ++ind;
        }
    }
}
auto main() -> signed {
    DynamicArray<elem> data;
    elem el;
    while (std::cin >> el.key) {
        char c;
		std::cin.get();
        while (std::cin.get(c) && c != '\n') {
            el.val.push_back(c);
        }
        data.push_back(std::move(el));
    }
    bucket_sort(data.begin(), data.end());
    for (const auto& [key, val] : data) {
        std::cout << key << '\t';
        for (char c : val) {
            std::cout << c;
        }
        std::cout << '\n';
    }
    return 0;
}
