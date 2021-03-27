#pragma once
#include <assert.h>
#include "container.hpp"
#include <algorithm>
#include <sstream>

template <typename T>
class Vector : public Container
{
    int m_len = 0;
    T* m_arr = nullptr;
    
    void realloc(int size);
    void copy(T* destination, T* source, int len);

    void swap(Vector& that);
    
    public:

    friend void PrintVector();

    Vector(                     params ss = {"TEMPORARY",       __FILE__, __LINE__, Container::num_of_objects + 1, TYPE::VECTOR});
    explicit Vector(int size,   params ss = {"TEMPORARY",       __FILE__, __LINE__, Container::num_of_objects + 1, TYPE::VECTOR});
    
    Vector(const Vector& rhs,   params ss = {"TEMPORARY",       __FILE__, __LINE__, Container::num_of_objects + 1, TYPE::VECTOR});
    Vector(Vector&& rhs,        params ss = {"TEMPORARY(THIEF)",__FILE__, __LINE__, Container::num_of_objects + 1, TYPE::VECTOR});
    Vector& operator=(const Vector& rhs);
    Vector& operator=(Vector&& rhs);
    ~Vector();

    T& operator[](int index);
    const T& operator[](int index) const;
    Vector& operator+=(const Vector& rhs);
    
    void setSize(int size) { assert(size <= m_len); realloc(size); }
    int  getSize() const override { return m_len; } 
};

template<typename T>
Vector<T>  operator+ (const Vector<T>& lhs, const Vector<T>& rhs)
{
    Vector<T> temp = lhs;
    temp += rhs;
    return temp;
}


void Printvector(Container& )
{
    std::stringstream res;

    //res << "type: " << m_ss.name << "\n";

}

template <typename T>
void Vector<T>::copy(T* dest, T* src, int len)
{
    for(int i = 0; i < len; i ++)
        dest[i] = src[i];
}

template <typename T>
void Vector<T>::realloc(int size)
{
    T* temp = new T [size] {};

    copy(temp, m_arr, m_len);
    
    delete [] m_arr;
    m_arr = temp;
}

template <typename T>
void Vector<T>::swap(Vector& that)
{
    std::swap(m_len, that.m_len);
    std::swap(m_arr, that.m_arr);
}

template <typename T>
Vector<T>::Vector(params ss)
: Container(ss)
{
    
}

template <typename T>
Vector<T>::Vector(int size, params ss)
: Container(ss), m_len(size), m_arr(new T [size] {}) 
{}

template <typename T>
Vector<T>::Vector(const Vector& rhs, params ss)
: Container(ss), m_len(rhs.m_len), m_arr(new T [rhs.m_len] {})
{
    std::copy(rhs.m_arr, rhs.m_arr + rhs.m_len, m_arr);
}

template <typename T>
Vector<T>::Vector(Vector&& rhs, params ss)
: Vector(ss)
{
    swap(rhs);
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
    if (this != &rhs)
    {
        Vector temp = rhs;
        swap(temp);
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& rhs)
{
    if (this != &rhs) swap(rhs);
    return *this;
}

template <typename T>
Vector<T>::~Vector()
{
    if(m_arr) delete [] m_arr;
}

template <typename T>
T& Vector<T>::operator[](int index)
{
    assert(0 <= index && index < m_len);
    return m_arr[index];
}

template <typename T>
const T& Vector<T>::operator[](int index) const
{
    assert(0 <= index && index < m_len);
    return m_arr[index];
}

template <typename T>
Vector<T>& Vector<T>::operator+=(const Vector& rhs)
{
    realloc(m_len + rhs.m_len);
    copy(&m_arr[m_len], rhs.m_arr, rhs.m_len);

    return *this;
}

// template <typename T>
// Vector<T> Vector<T>::operator+(const Vector& rhs)
// {
//     Vector<T> temp = *this;
//     temp += rhs;
//     return temp;
// }





/*

Vector::Vector(const Vector& that)
: m_len(that.m_len),
  m_buffer(new int [that.m_len])
{
    std::copy(that.m_buffer, that.m_buffer + that.m_len, m_buffer);
}


or make function COPY();


Vector::Vector(Vector&& that)
: Vector()
{
    swap(that);
}

Vector& Vector::operator=(const Vector& that)
{
    
    --
    | Vector temp = that;  
    | swap(temp);
    |
    |
    | --------optional: 
    |       
    |       *this = Vector(that);
    |
    |       Vector temp = that;
    |       *this = reinterpret_cast<Vector&&>(temp);
    |
    |
    |       another version:    Vector temp = that;
    |                           *this = std::move(temp);
    --
    
    return *this;
}

Vector& Vector::operator=(Vector&& that)
{
    swap(that);
    returb *this;
}

TODO::
add move constructor and copy constructor and also realise rule of 5 
*/