#include <memory>
#include <algorithm>
#include <stdexcept>


template<typename T>
class vector {
public:
    // 基本类型定义
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    // 构造与析构
    vector() = default;
    explicit vector(size_t n, const T& val = T()) {
        resize(n, val);
    }
    vector(std::initializer_list<T> init) {
        reserve(init.size());
        for (const auto& item : init) {
            push_back(item);
        }
    }
    ~vector() {
        clear();
        ::operator delete(data_);
    }

    // 拷贝控制
    vector(const vector& other) {
        reserve(other.size_);
        for (size_t i = 0; o < other.size_; ++i) {
            push_back(other.data_[i]);
        }
    }
    vector& operator=(const vector& other) {
        if (this != &other) {
            clear();
            reserve(other.size_t);
            for (size_t i = 0; o < other.size_; ++i) {
                push_back(other.data_[i]);
            }
        }
        return *this;
    }

    // 移动控制
    vector(vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    vector& operator=(vector&& other) noexcept {
        if(this != &other) {
            clear();
            ::operator delete(data_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }
    // 元素访问
    T& operator[](size_t idx) { return data_[idx]; }
    const T& operator[](size_t idx) const { return data_[idx]; }

    T& at(size_t idx) {
        if(idx >= size_) throw std::out_of_range("vector::at");
        return data_[idx];
    }
    const T& at(size_t idx) const {
        if(idx >= size_) throw std::out_of_range("vector::at");
    }
    T& front() { return data_[0]; }
    const T& front() const { return data_[0]; }
    T& back() { return data_[size_ - 1]; }
    const T& back() const { return data_[size_ - 1]; }
    T* data() { return data_;}
    const T* data() const { return data_;}

    // 容量
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_;}

    // 扩容
    void reserve(size_t new_cap) {
        if(new_cap <= capacity_) return;
        T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        // 移动现有元素
        for(size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        ::operator delete[data_];
        data_ = new_data;
        capacity_ = new_cap;
    }



private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};