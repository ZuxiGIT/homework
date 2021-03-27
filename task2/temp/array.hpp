#pragma once
#include <assert.h>
#include <stdio.h>
#include "container.hpp"

template <typename T>
class Array : public Container
{
    const static int m_max = 1000;
    T m_arr[m_max] = {};
    int m_len = 0;

    public:

    Array() = default;
    Array(int len);
    Array(const Array& rhs);
    ~Array();

    Array& operator+=(int num);
    Array  operator+ (int num) { Array* temp = new Array; *temp =  *this; *temp+=num; return *temp; }
    
    Array& operator+=(const Array& rhs);
    Array  operator+ (const Array& rhs) { Array* temp = new Array; *temp = *this; *temp+=rhs; return *temp; }
    
    Array& operator=(const Array& rhs);
    
    void setSize(int size) { assert(!(0 <= size && size < m_max)); m_len = size;}
    int  getSize() const override { return m_len; }
    
    T& operator[](int index);
    bool append(int num);

};



template <typename T>
Array<T>::Array(const Array& rhs)
: m_len(rhs.m_len)
{
    for(int i = 0; i < m_len; i++)
        m_arr[i] = rhs.m_arr[i];
}

template <typename T>
Array<T>::Array(int len)
{
    assert(m_len < m_max);
    m_len = len;
}

template <typename T>
Array<T>::~Array()
{
    for(int i = 0; i < m_len; i++)
    {
        m_arr[i] = 0;
    }
}

template <typename T>
Array<T>& Array<T>::operator+=(int num)
{
    append(num);
    return *this;
}

template <typename T>
Array<T>& Array<T>::operator+=(const Array& rhs)
{
    if(m_len + rhs.m_len < m_max)
    {
        for(int i = m_len; i < m_len + rhs.m_len; i ++)
            m_arr[i] = rhs.m_arr[i];
        m_len += rhs.m_len;
    }
    else
        assert(!"can't concat");
    
    return *this;
}


template <typename T>
Array<T>& Array<T>::operator=(const Array& rhs)
{
    m_len = rhs.m_len;
    for(int i = 0; i < m_len; i++)
        m_arr[i] = rhs.m_arr[i];

    return *this;
}


template <typename T>
T& Array<T>::operator[] (int index)
{
    assert(index >= 0 && index < m_len);
    return m_arr[index];
}

template <typename T>
bool Array<T>::append(int num)
{
    if( m_len == m_max)
    {
        fprintf(stderr, "Array is full\n");
        return false;
    }
    m_arr[m_len++] = num;
    return true;
}
