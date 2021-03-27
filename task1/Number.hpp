#pragma once

#include "Log.hpp"


extern int num_of_temporary;

struct prop
{
    const char* name = nullptr;
    const char* file = nullptr;
    int line = -1;
    int number = num_of_temporary--; 
    const char* constructor = nullptr;
};

class Number 
{
    int val;
    prop info;
    static int m_compare_number;
    static int m_copy_number;
public:
    friend void Log::printProp(const Number& ss);

    static int num_of_objects;
    static int max_size;

    static void printStat();

    Number(                   prop ss = {"TEMPORARY", __FILE__ , -1, num_of_objects + 1});
    Number(int num,           prop ss = {"TEMPORARY", __FILE__ , -1, num_of_objects + 1});
    Number(double num,        prop ss = {"TEMPORARY", __FILE__ , -1, num_of_objects + 1});
    Number(const char* str,   prop ss = {"TEMPORARY", __FILE__ , -4, num_of_objects + 1});
    Number(const Number& rhs, prop ss = {"TEMPORARY", __FILE__ , -5, num_of_objects + 1});

    ~Number();

    int getval() const { return val; }


    friend Number operator+(const Number& lhs, const Number& rhs);
    friend Number operator-(const Number& lhs, const Number& rhs);
    friend Number operator*(const Number& lhs, const Number& rhs);
    friend Number operator/(const Number& lhs, const Number& rhs);
    friend Number operator%(const Number& lhs, const Number& rhs);
    friend Number operator&(const Number& lhs, const Number& rhs);
    friend Number operator|(const Number& lhs, const Number& rhs);


    Number& operator+()                   { Log log(__PRETTY_FUNCTION__); log.printProp(*this); return *this; }
    Number  operator-()                   { Log log(__PRETTY_FUNCTION__); log.printProp(*this); return Number(-val); }
    Number& operator++()                  { Log log(__PRETTY_FUNCTION__); log.printProp(*this); val++; return *this; }
    Number  operator++(int)               { Log log(__PRETTY_FUNCTION__); log.printProp(*this); Number temp {*this}; ++(*this); return temp; }
    Number& operator--()                  { Log log(__PRETTY_FUNCTION__); log.printProp(*this); val--; return *this; }
    Number  operator--(int)               { Log log(__PRETTY_FUNCTION__); log.printProp(*this); Number temp {*this}; --(*this); return temp; }
    Number& operator= (const Number& rhs) { Log log(__PRETTY_FUNCTION__); log.printProp(*this); val = rhs.val; return *this; }

    bool operator==(const Number& rhs) { m_compare_number++; Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); return val == rhs.val; }
    bool operator!=(const Number& rhs) { m_compare_number++; Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); return !(*this == rhs); }
    bool operator> (const Number& rhs) { m_compare_number++; Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); return val > rhs.val; }
    bool operator< (const Number& rhs) { m_compare_number++; Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); return val < rhs.val; }
    bool operator>=(const Number& rhs) { m_compare_number++; Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); return !(*this < rhs);}
    bool operator<=(const Number& rhs) { m_compare_number++; Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); return !(*this > rhs);}

/*
    Number& operator+=(const Number& rhs) { P("operator+=") *this = *this + rhs; return *this; }
    Number& operator-=(const Number& rhs) { P("operator-=") *this = *this - rhs; return *this; }
    Number& operator*=(const Number& rhs) { P("operator*=") *this = *this * rhs; return *this; }
    Number& operator/=(const Number& rhs) { P("operator/=") *this = *this / rhs; return *this; }
    Number& operator%=(const Number& rhs) { P("operator%%=")*this = *this % rhs; return *this; }
    Number& operator&=(const Number& rhs) { P("operator&=") *this = *this & rhs; return *this; }
    Number& operator|=(const Number& rhs) { P("operator|=") *this = *this | rhs; return *this; }
*/
    
};
