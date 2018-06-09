#pragma once

#include <memory>

namespace atl {
    template <typename T, typename Allocator = std::allocator<T>> class vector;

template <class T, bool is_const>
class VectorIterator
{
public:
    //ASK: можно/нужно вынести в iterator_traits?
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t ;
    using value_type        = typename std::conditional<is_const, const T, T>::type;
    using reference         = typename std::conditional<is_const, const T&, T&>::type;
    using pointer           = typename std::conditional<is_const, const T*, T*>::type;
    using iterator_category = std::random_access_iterator_tag ;

    VectorIterator() = delete;
    VectorIterator(const VectorIterator& other) : VectorIterator(other.start_ptr_, other.size_, other.pos_) {}
    VectorIterator& operator=(const VectorIterator& rhs);

    VectorIterator& operator++(); //prefix increment
    const VectorIterator operator++(int); //postfix increment

    VectorIterator& operator--(); //prefix decrement
    //ASK: why const?
    const VectorIterator operator--(int); //postfix decrement

    reference operator*() const;
    pointer operator->();

    template <class U, bool is_const_u, class F, bool is_const_f>
    friend bool operator==(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

    template <class U, bool is_const_u, class F, bool is_const_f>
    friend bool operator!=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

    VectorIterator& operator+=(size_type);

    template<class U, bool is_const_u>
    friend VectorIterator<U, is_const_u> operator+(const VectorIterator<U, is_const_u>& lhs,
                                                   typename VectorIterator<U, is_const_u>::size_type);

    template<class U, bool is_const_u>
    friend VectorIterator<U, is_const_u> operator+(typename VectorIterator<U, is_const_u>::size_type,
                                                   const VectorIterator<U, is_const_u>& rhs);

    VectorIterator& operator-=(size_type);

    template<class U, bool is_const_u>
    friend VectorIterator<U, is_const_u> operator-(const VectorIterator<U, is_const_u>& lhs,
                                                   typename VectorIterator<U, is_const_u>::size_type);

    template <class U, bool is_const_u, class F, bool is_const_f>
    friend typename VectorIterator<U, is_const_u>::difference_type operator-(VectorIterator<U, is_const_u>& lhs,
                                                                             VectorIterator<F, is_const_f>& rhs);

    reference operator[](size_type) const;

    template <class U, bool is_const_u, class F, bool is_const_f>
    friend bool operator<(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

    template <class U, bool is_const_u, class F, bool is_const_f>
    friend bool operator>(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

    template <class U, bool is_const_u, class F, bool is_const_f>
    friend bool operator<=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

    template <class U, bool is_const_u, class F, bool is_const_f>
    friend bool operator>=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

private:
    explicit VectorIterator(pointer elem_ptr, difference_type size, difference_type pos = 0)
            : start_ptr_(elem_ptr), size_(size), pos_(pos) {}

    pointer start_ptr_;
    difference_type size_;
    difference_type pos_;

    friend vector<T>;
};

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator=(const VectorIterator& rhs)
{
    if (this != &rhs) {
        start_ptr_ = rhs.start_ptr_;
        size_ = rhs.size_;
        pos_ = rhs.pos_;
    }

    return *this;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator++()
{
    pos_++;
    return *this;
}

template<class T, bool is_const>
const VectorIterator<T, is_const> VectorIterator<T, is_const>::operator++(int)
{
    VectorIterator<T, is_const> tmp(*this); //copy
    operator++();
    return tmp;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator--()
{
    pos_--;
    return *this;
}

template<class T, bool is_const>
const VectorIterator<T, is_const> VectorIterator<T, is_const>::operator--(int)
{
    VectorIterator<T, is_const> tmp(*this); //copy
    operator--();
    return tmp;
}

template<class T, bool is_const>
typename VectorIterator<T, is_const>::reference VectorIterator<T, is_const>::operator*() const
{
    return *(start_ptr_ + pos_);
}

template<class T, bool is_const>
typename VectorIterator<T, is_const>::pointer VectorIterator<T, is_const>::operator->()
{
    return start_ptr_ + pos_;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator==(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
    return lhs.pos_ == rhs.pos_;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator!=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
    return lhs.pos_ != rhs.pos_;
}

template<class U, bool is_const_u>
VectorIterator<U, is_const_u> operator+(const VectorIterator<U, is_const_u>& lhs,
                                        typename VectorIterator<U, is_const_u>::size_type rhs)
{
    return VectorIterator<U, is_const_u>(lhs.start_ptr_, lhs.size_, lhs.pos_ + rhs);
}

template<class U, bool is_const_u>
VectorIterator<U, is_const_u> operator+(typename VectorIterator<U, is_const_u>::size_type lhs,
                                        const VectorIterator<U, is_const_u>& rhs)
{
    return rhs + lhs;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator+=(VectorIterator::size_type n)
{
    pos_ += n;
    return *this;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator-=(VectorIterator::size_type n)
{
    pos_ -= n;
    return *this;
}

template<class U, bool is_const_u>
VectorIterator<U, is_const_u> operator-(const VectorIterator<U, is_const_u>& lhs,
                                        typename VectorIterator<U, is_const_u>::size_type rhs)
{
    return VectorIterator<U, is_const_u>(lhs.start_ptr_, lhs.size_, lhs.pos_ - rhs);
}

template<class U, bool is_const_u, class F, bool is_const_f>
typename VectorIterator<U, is_const_u>::difference_type operator-(VectorIterator<U, is_const_u>& lhs,
                                                                  VectorIterator<F, is_const_f>& rhs)
{
    return lhs.pos_ - rhs.pos_;
}

template<class T, bool is_const>
typename VectorIterator<T, is_const>::reference VectorIterator<T, is_const>::operator[](VectorIterator::size_type n) const
{
    return *(start_ptr_ + n);
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator<(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
    return lhs.pos_ < rhs.pos_;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator>(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
    return lhs.pos_ > rhs.pos_;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator<=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
    return lhs.pos_ <= rhs.pos_;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator>=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
    return lhs.pos_ >= rhs.pos_;
}

}//namespace atl