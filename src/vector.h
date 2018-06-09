#pragma once

#include <memory>
#include <cmath>
#include <utility>
#include <iterator>
#include <algorithm>
#include "vector_iterator.h"
#include <initializer_list>

//Alexey template library
namespace atl {

//FD
//template <typename T, bool is_const> class Iterator;
//template <typename T, typename Allocator = std::allocator<T>> class vector;

//operators
//template <class T, class Allocator>
//    bool operator==(const vector<T,Allocator>& lhs,const vector<T,Allocator>& rhs);
//template <class T, class Allocator>
//bool operator< (const vector<T,Allocator>& x,const vector<T,Allocator>& y);
//template <class T, class Allocator>
//bool operator!=(const vector<T,Allocator>& x,const vector<T,Allocator>& y);
//template <class T, class Allocator>
//bool operator> (const vector<T,Allocator>& x,const vector<T,Allocator>& y);
//template <class T, class Allocator>
//bool operator>=(const vector<T,Allocator>& x,const vector<T,Allocator>& y);
//template <class T, class Allocator>
//bool operator<=(const vector<T,Allocator>& x,const vector<T,Allocator>& y);
//
//template <class T, class Allocator>
//void swap(vector<T,Allocator>& x, vector<T,Allocator>& y);
//
//template <class Allocator> class vector<bool,Allocator>;
//
//// hash support
//template <class T> struct hash;
//template <class Allocator> struct hash<vector<bool, Allocator> >;


template <class T, class Allocator>
class vector {
public:
    // types:
    using value_type                                   = T;
    using reference                                    = value_type&;
    using const_reference                              = const value_type&;
    using size_type                                    = std::size_t;
    using difference_type                              = std::ptrdiff_t ;

    using allocator_type                               = Allocator;
    using pointer                                      = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer                                = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator                                     = VectorIterator<T, false>;
    using const_iterator                               = VectorIterator<T, true>;
    using reverse_iterator                             = std::reverse_iterator<iterator>;
    using const_reverse_iterator                       = std::reverse_iterator<const_iterator>;

    // construct/copy/destroy:
    explicit vector(const Allocator& alloc = Allocator());
    explicit vector(size_type size);
    vector(size_type size, const T& value, const Allocator& = Allocator());
    template <class InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>
    vector(InputIterator first, InputIterator last,const Allocator& = Allocator());
//    vector(const vector<T,Allocator>& x);
//    vector(vector&&);
//    vector(const vector&, const Allocator&);
//    vector(vector&&, const Allocator&);
//    vector(initializer_list<T>, const Allocator& = Allocator());
//
    ~vector();
//    vector<T,Allocator>& operator=(const vector<T,Allocator>& x);
//    vector<T,Allocator>& operator=(vector<T,Allocator>&& x);
//    vector& operator=(initializer_list<T>);
    template <class InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>
    void assign(InputIterator first, InputIterator last);

    void assign(size_type n, const T& elem);
    void assign(std::initializer_list<T>);
    allocator_type get_allocator() const noexcept;

    // iterators:
    iterator                begin() noexcept;
    const_iterator          begin() const noexcept;
    iterator                end() noexcept;
    const_iterator          end() const noexcept;
//
//    reverse_iterator        rbegin() noexcept;
//    const_reverse_iterator  rbegin() const noexcept;
//    reverse_iterator        rend() noexcept;
//    const_reverse_iterator  rend() const noexcept;
//
//    const_iterator          cbegin() noexcept;
//    const_iterator          cend() noexcept;
//    const_reverse_iterator  crbegin() const noexcept;
//    const_reverse_iterator  crend() const noexcept;
//
    // capacity:
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void      resize(size_type sz);
    void      resize(size_type sz, const T& elem);
    size_type capacity() const noexcept;
    bool      empty() const noexcept;
    void      reserve(size_type cp);
    void      shrink_to_fit();

    // element access:
    reference       operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference       at(size_type pos);
    const_reference at(size_type pos) const;
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;

    //data access
    T*       data() noexcept;
    const T* data() const noexcept;

    // modifiers:
    template <class... Args> void emplace_back(Args&& ...args);
    void push_back(const T& elem);
    void push_back(T&& elem);
    void pop_back();
//
//    template <class... Args> iterator emplace(const_iterator position, Args&&... args);
//    iterator insert(const_iterator position, const T& x);
//    iterator insert(const_iterator position, T&& x);
//    iterator insert(const_iterator position, size_type n, const T& x);
//    template <class InputIterator>
//    iterator insert (const_iterator position, InputIterator first,
//                     InputIterator last);
//    iterator insert(const_iterator position, initializer_list<T>);
//
//    iterator erase(const_iterator position);
//    iterator erase(const_iterator first, const_iterator last);
//    void     swap(vector<T,Allocator>&);
    void     clear() noexcept;

private:
    static constexpr double     INCREASE_CAPACITY_FACTOR = 1.5;
    const  size_type            MIN_CAPACITY             = 10;

    //ASK:???
//    static constexpr size_type  MIN_CAPACITY             = 10;

    Allocator allocator_;
    pointer data_;
    size_type size_;
    size_type capacity_;

    void initialize_default();
    void reserve_for_push();
    void copy_to_another_ptr(pointer);
};


template<class T, class Allocator>
vector<T, Allocator>::vector(const Allocator& alloc)
         : allocator_(alloc),
           data_ (std::allocator_traits<Allocator>::allocate(allocator_, MIN_CAPACITY)),
           size_(0),
           capacity_(MIN_CAPACITY) {}

template<class T, class Allocator>
vector<T, Allocator>::vector(vector::size_type size)
         : allocator_(Allocator()),
           data_(std::allocator_traits<Allocator>::allocate(allocator_, size)),
           size_(size),
           capacity_(size)
{
    initialize_default();
}

template<class T, class Allocator>
vector<T, Allocator>::vector(vector::size_type size, const T& value, const Allocator& allocator)
         :  allocator_(allocator),
            data_(std::allocator_traits<Allocator>::allocate(allocator_, size)),
            size_(size),
            capacity_(size)
{
    //TODO: rmk with iterators
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const T&>(value));
    }
}

template<class T, class Allocator>
template<class InputIterator, class>
vector<T, Allocator>::vector(InputIterator first, InputIterator last, const Allocator& alloc)
         : allocator_(alloc),
           data_(std::allocator_traits<Allocator>::allocate(allocator_, std::distance(first, last))),
           size_(static_cast<size_type >(std::distance(first, last))),
           capacity_(size_)
{
    int i = 0;
    for (auto it = first; it != last; it++, i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
    }
}


template<class T, class Allocator>
vector<T, Allocator>::~vector()
{
    for (size_type i = 0; i < capacity_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
}

template<class T, class Allocator>
void vector<T, Allocator>::initialize_default()
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i);
    }
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](vector::size_type n)
{
    return data_[n];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](vector::size_type n) const {
    return data_[n];
}

template<class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept
{
    return capacity_;
}

template<class T, class Allocator>
bool vector<T, Allocator>::empty() const noexcept
{

    return begin() ==  end();
}

template<class T, class Allocator>
typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const noexcept
{
    return allocator_;
}

template<class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept
{
    return size_;
}

template<class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const noexcept
{
    return std::allocator_traits<Allocator>::max_size(allocator_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(vector::size_type pos)
{
    if (pos < 0 || size() <= pos) {
        throw std::out_of_range("Index out of range");
    }
    return data_[pos];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(vector::size_type pos) const
{
    if (pos < 0 || size() <= pos) {
        throw std::out_of_range("Index out of range");
    }
    return data_[pos];
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::front()
{
    return data_[0];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const
{
    return data_[0];
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::back()
{
    return data_[size_ - 1];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const
{
    return data_[size_ - 1];
}

template<class T, class Allocator>
T* vector<T, Allocator>::data() noexcept
{
    return data_;
}

template<class T, class Allocator>
const T* vector<T, Allocator>::data() const noexcept
{
    return data_;
}

template<class T, class Allocator>
void vector<T, Allocator>::push_back(const T& elem)
{
    reserve_for_push();
    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, std::forward<const T&>(elem));
    size_++;
}

template<class T, class Allocator>
void vector<T, Allocator>::push_back(T&& elem)
{
    reserve_for_push();
    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, std::forward<T&&>(elem));
    size_++;
}

template<class T, class Allocator>
void vector<T, Allocator>::resize(typename vector<T, Allocator>::size_type sz)
{
    auto new_capacity = std::max(sz, MIN_CAPACITY);

    if (sz < size_) {
        for (auto i = sz; i < size_; i++) {
            std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
        }

        size_ = sz;
        return;
    }

    auto new_data = std::allocator_traits<Allocator>::allocate(allocator_, new_capacity);

    copy_to_another_ptr(new_data);
    for (size_type i = capacity_; i < new_capacity; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, new_data + i);
    }

    std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
}

template<class T, class Allocator>
void vector<T, Allocator>::resize(typename vector<T, Allocator>::size_type sz, const T& elem)
{
    //TODO: remove copypaste

    auto new_capacity = std::max(sz, MIN_CAPACITY);
    if (sz < size_) {
        for (auto i = sz; sz < size_; i++) {
            std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
        }

        size_ = sz;
        return;
    }

    auto new_data = std::allocator_traits<Allocator>::allocate(allocator_, new_capacity);

    copy_to_another_ptr(new_data);
    for (size_type i = capacity_; i < new_capacity; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, new_data + i, elem);
    }

    std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
}


template<class T, class Allocator>
void vector<T, Allocator>::reserve(vector::size_type capacity)
{
    if (capacity <= capacity_) {
        return;
    }
    resize(capacity);
}

template<class T, class Allocator>
void vector<T, Allocator>::reserve_for_push()
{
    if (capacity_ - size_ > 0) {
        return;
    }

    auto new_capacity = static_cast<size_type >(std::floor(capacity_ * INCREASE_CAPACITY_FACTOR));
    reserve(new_capacity);
}

template<class T, class Allocator>
void vector<T, Allocator>::pop_back()
{
    if (size_ > 0) {
        --size_;
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + size_);
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::copy_to_another_ptr(vector::pointer new_data)
{
    for (size_type i = 0; i < capacity_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, new_data + i, std::move(data_[i]));
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit()
{
    resize(size_);
}

template<class T, class Allocator>
template<class... Args>
void vector<T, Allocator>::emplace_back(Args&& ...args)
{
    reserve_for_push();
    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, std::forward<Args&&>(args)...);
    size_++;
}

template<class T, class Allocator>
void vector<T, Allocator>::clear() noexcept
{
    resize(0);
}

template<class T, class Allocator>
typename vector<T,Allocator>::iterator vector<T, Allocator>::begin() noexcept
{
    return iterator(data_, size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept
{
    return const_iterator(data_, size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept
{
    return iterator(data_, size_, size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept
{
    return const_iterator(data_, size_, size_);
}

template<class T, class Allocator>
void vector<T, Allocator>::assign(vector::size_type n, const T& elem)
{
    clear();
    reserve(n);

    for (size_type i = 0; i < n; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, elem);
    }

    size_ = n;
}

template<class T, class Allocator>
template<class InputIterator, class>
void vector<T, Allocator>::assign(InputIterator first, InputIterator last)
{
    clear();
    auto size = static_cast<size_type>(std::distance(first, last));
    reserve(size);

    size_type i =  0;
    for (auto it = first; it != last; it++, i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
    }

    size_ = size;
}

template<class T, class Allocator>
void vector<T, Allocator>::assign(std::initializer_list<T> init_list)
{
    assign(init_list.begin(), init_list.end());
}

} //namespace atl
