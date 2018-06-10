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
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t ;

    using allocator_type         = Allocator;
    using pointer                = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator               = VectorIterator<T, false>;
    using const_iterator         = VectorIterator<T, true>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // construct/copy/destroy:
    explicit vector(const Allocator& alloc = Allocator());
    explicit vector(size_type size);
    vector(size_type size, const T& value, const Allocator& = Allocator());

    template <class InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>
    vector(InputIterator first, InputIterator last,const Allocator& = Allocator());
    //copy constructors
    vector(const vector<T,Allocator>& other);
    vector(const vector&, const Allocator&);
    //move constructors
    vector(vector&&) noexcept ;
    vector(vector&&, const Allocator&);

    vector(std::initializer_list<T>, const Allocator& = Allocator());
    //Destructor
    ~vector();

    //operators
    vector<T,Allocator>& operator=(const vector<T,Allocator>& rhs);
    //ASK: why move operators should be noexcept?
    vector<T,Allocator>& operator=(vector<T,Allocator>&& rhs) noexcept;
    vector& operator=(std::initializer_list<T>);

    template <class InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>
    void assign(InputIterator first, InputIterator last);

    void assign(size_type n, const T& elem);
    void assign(std::initializer_list<T>);
    allocator_type get_allocator() const noexcept;

    // iterators:
    iterator                begin() noexcept;
    const_iterator          begin() const noexcept;
    iterator                end()   noexcept;
    const_iterator          end()   const noexcept;

    reverse_iterator        rbegin() noexcept;
    const_reverse_iterator  rbegin() const noexcept;
    reverse_iterator        rend() noexcept;
    const_reverse_iterator  rend() const noexcept;

    const_iterator          cbegin() noexcept;
    const_iterator          cend() noexcept;
    const_reverse_iterator  crbegin() const noexcept;
    const_reverse_iterator  crend() const noexcept;

    // capacity:
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void      resize(size_type new_size);
    void      resize(size_type new_size, const T& elem);
    size_type capacity() const noexcept;
    bool      empty() const noexcept;
    void      reserve(size_type capacity);
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
    pointer       data() noexcept;
    const_pointer data() const noexcept;

    // modifiers:
    template <class... Args> void emplace_back(Args&& ...args);
    void push_back(const T& elem);
    void push_back(T&& elem);
    void pop_back();

    template <class... Args> iterator emplace(const_iterator position, Args&&... args);
    iterator insert(const_iterator position, const T& elem);
    iterator insert(const_iterator position, T&& elem);
    iterator insert(const_iterator position, size_type n, const T& elem);
    template <class InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>
    iterator insert (const_iterator position, InputIterator first, InputIterator last);
    iterator insert(const_iterator position, std::initializer_list<T>);
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

    void initialize_default(size_type from = 0);
    void reserve_for_push(size_type size = 1);
    void move_to_another_ptr(pointer);

    void copy_from_another_vector(const vector<T>& other);

    template<class It, class = typename std::iterator_traits<It>::iterator_category>
    void fill_from_iterator(It first, It last);
    void deallocate_data();
    void destruct_data(size_type from = 0);
    void shift_right(const_iterator pos, size_type n = 1);
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
vector<T, Allocator>::vector(const vector<T, Allocator>& other)
     : allocator_(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())),
       data_(std::allocator_traits<Allocator>::allocate(allocator_, other.capacity_)),
       size_(other.size_),
       capacity_(other.capacity_)
{
    copy_from_another_vector(other);
}

template<class T, class Allocator>
vector<T, Allocator>::vector(const vector& other, const Allocator& alloc)
    : allocator_(alloc),
      data_(std::allocator_traits<Allocator>::allocate(allocator_, other.capacity_)),
      size_(other.size_),
      capacity_(other.capacity_)
{

    copy_from_another_vector(other);
}

template<class T, class Allocator>
vector<T, Allocator>::vector(vector&& other) noexcept
        :  allocator_(Allocator()),
           data_(std::allocator_traits<Allocator>::allocate(allocator_, other.capacity_)),
           size_(other.size_),
           capacity_(other.capacity_)
{
    other.data_ = nullptr;
}

template<class T, class Allocator>
vector<T, Allocator>::vector(vector&& other, const Allocator& alloc)
     :  allocator_(alloc),
        data_(std::allocator_traits<Allocator>::allocate(allocator_, other.capacity_)),
        size_(other.size_),
        capacity_(other.capacity_)
{
    other.data_ = nullptr;
}

template<class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<T> ilist, const Allocator& alloc)
        : allocator_(alloc),
          data_(std::allocator_traits<Allocator>::allocate(allocator_, ilist.size())),
          size_(ilist.size()),
          capacity_(ilist.size())
{
    fill_from_iterator(ilist.begin(), ilist.end());
}

template<class T, class Allocator>
vector<T, Allocator>::~vector()
{
    deallocate_data();
}

template<class T, class Allocator>
void vector<T, Allocator>::initialize_default(size_type from)
{
    for (size_type i = from; i < size_; i++) {
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
    return size_ == 0;
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
typename vector<T, Allocator>::pointer vector<T, Allocator>::data() noexcept
{
    return data_;
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_pointer vector<T, Allocator>::data() const noexcept
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
void vector<T, Allocator>::resize(typename vector<T, Allocator>::size_type new_size)
{
    auto needed_capacity = std::max(new_size, MIN_CAPACITY);

    if (new_size == size_) {
        return;
    }

    if (new_size < size_) {
        destruct_data(new_size);
        size_ = new_size;
        return;
    }

    if (needed_capacity > capacity_) {
        auto new_data = std::allocator_traits<Allocator>::allocate(allocator_, needed_capacity);
        move_to_another_ptr(new_data);

        std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
        data_= new_data;
        capacity_ = needed_capacity;
    }

    initialize_default(size_);
    size_ = new_size;
}

template<class T, class Allocator>
void vector<T, Allocator>::resize(typename vector<T, Allocator>::size_type new_size, const T& elem)
{
    auto needed_capacity = std::max(new_size, MIN_CAPACITY);

    if (new_size == size_) {
        return;
    }

    if (new_size < size_) {
        destruct_data(new_size);
        size_ = new_size;
        return;
    }

    if (needed_capacity > capacity_) {
        auto new_data = std::allocator_traits<Allocator>::allocate(allocator_, needed_capacity);
        move_to_another_ptr(new_data);

        std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
        data_= new_data;
        capacity_ = needed_capacity;
    }


    for (size_type i = size_; i < new_size; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, elem);
    }

    size_ = new_size;
}

template<class T, class Allocator>
void vector<T, Allocator>::reserve(vector<T, Allocator>::size_type capacity)
{
    if (capacity <= capacity_) {
        return;
    }

    auto new_data = std::allocator_traits<Allocator>::allocate(allocator_, capacity);
    move_to_another_ptr(new_data);

    std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
    data_= new_data;
    capacity_ = capacity;
    initialize_default(size_);
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
template<class... Args>
typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(vector::const_iterator position, Args&&... args)
{
    shift_right(position);
    std::allocator_traits<Allocator>::construct(allocator_, data_ + position.pos_, std::forward<Args&&>(args)...);
    size_++;
    return iterator(data_, size_, position.pos_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(vector::const_iterator position, const T& elem)
{
    shift_right(position);
    std::allocator_traits<Allocator>::construct(allocator_, data_ + position.pos_, std::forward<const T&>(elem));
    size_++;
    return iterator(data_, size_, position.pos_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(vector::const_iterator position, T&& elem)
{
    shift_right(position);
    std::allocator_traits<Allocator>::construct(allocator_, data_ + position.pos_, std::forward<T&&>(elem));
    size_++;
    return iterator(data_, size_, position.pos_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(vector::const_iterator position,
                                                                     vector::size_type n, const T& elem)
{
    shift_right(position, n);

    for (size_type i = position.pos_; i < position.pos_ + n; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const T&>(elem));
    }

    size_ += n;
    return iterator(data_, size_, position.pos_);
}

template<class T, class Allocator>
template<class InputIterator, class>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(vector::const_iterator position, InputIterator first, InputIterator last)
{
    size_type size = last - first;
    shift_right(position, size);

    size_type  i = position.pos_;

    for (auto it = first; it != last;i++, it++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
    }

    size_ += size;
    return iterator(data_, size_, position.pos_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(vector::const_iterator position, std::initializer_list<T> ilist)
{
    return insert(position, ilist.begin(), ilist.end());
}


template<class T, class Allocator>
void vector<T, Allocator>::move_to_another_ptr(vector::pointer new_data)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, new_data + i, std::move(data_[i]));
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::copy_from_another_vector(const vector<T>& other)
{
    int i = 0;
    for (const auto& val : other) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, val);//copy
        ++i;
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::reserve_for_push(size_type size)
{
    auto needed_capacity = size_ + size;

    if (capacity_  > needed_capacity) {
        return;
    }

    //small kostyl
    size_type new_capacity;

    if (size == 1) {
        if (capacity_ == 0) {
            capacity_ = MIN_CAPACITY;
        } else {
            new_capacity = static_cast<size_type>(std::floor(capacity_ * INCREASE_CAPACITY_FACTOR));
        }
    } else {
        new_capacity = needed_capacity;
    }


    reserve(new_capacity);
}

template<class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit()
{
    if (capacity_ == size_) {
        return;
    }

    auto new_data = std::allocator_traits<Allocator>::allocate(allocator_, size_);
    move_to_another_ptr(new_data);
    std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
    data_= new_data;
    capacity_ = size_;
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
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class T, class Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() noexcept
{
    return const_iterator(data_, size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() noexcept
{
    return const_iterator(data_, size_, size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}


template<class T, class Allocator>
void vector<T, Allocator>::assign(vector::size_type n, const T& elem)
{
    destruct_data();
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
    destruct_data();
    auto size = static_cast<size_type>(std::distance(first, last));
    reserve(size);

    fill_from_iterator(first, last);
    size_ = size;
}

template<class T, class Allocator>
void vector<T, Allocator>::assign(std::initializer_list<T> init_list)
{
    assign(init_list.begin(), init_list.end());
}

template<class T, class Allocator>
template<class It, class>
void vector<T, Allocator>::fill_from_iterator(It first, It last)
{
    size_type i =  0;
    for (auto it = first; it != last; it++, i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
    }
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector<T, Allocator>& rhs)
{
    deallocate_data();
    allocator_ = rhs.allocator_;
    capacity_  = rhs.capacity_;
    size_      = rhs.size_;

    data_ = std::allocator_traits<Allocator>::allocate(allocator_, rhs.capacity());
    copy_from_another_vector(rhs);

    return *this;
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector<T, Allocator>&& rhs) noexcept
{
    deallocate_data();
    allocator_ = std::move(rhs.allocator_);
    capacity_  = rhs.capacity_;
    size_      = rhs.size_;

    data_ = std::move(rhs.data_);

    return *this;
}

//ATTENTION leaves container in non-consistent state
template<class T, class Allocator>
void vector<T, Allocator>::deallocate_data()
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(std::initializer_list<T> ilist)
{
    assign(std::move(ilist));
    return *this;
}

template<class T, class Allocator>
void vector<T, Allocator>::destruct_data(size_type from)
{
    for (size_type i = from; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::shift_right(vector::const_iterator pos, size_type n)
{
    reserve_for_push(n);

    for (auto it = end() + n - 1; it != pos + n - 1; it--) {
        std::allocator_traits<Allocator>::construct(allocator_, &*it, std::move(*(it-n)));
        std::allocator_traits<Allocator>::destroy(allocator_, &*(it-n));
    }
}

} //namespace atl
