#pragma once
#include "logger.hpp"
// #include <string>

//#include "Log.hpp"


extern int num_of_temporary;

struct prop
{
    const char* name = nullptr;
    const char* file = nullptr;
    int line = -1;
    int current_number = num_of_temporary--; 
    const char* constructor = nullptr;
    int origin_node = 0;
    const void* address = nullptr;
    int overall_number = 0;

    std::string getGraphNode();
    std::string getGraphEdge();
};


class Number : Obj2log
{
    int val;
    prop info;
    static int m_compare_number;
    static int m_copy_number;
public:
    //friend void Log::printProp(const Number& ss);

    static int num_of_objects;
    static int max_size;

    virtual std::string createGraphNode() override   { return info.getGraphNode(); } 
    virtual std::string createHistoryEdge() override { return info.getGraphEdge(); }

    static void printStat();

    Number(                   prop ss = {"TEMPORARY", __FILE__ , -1, num_of_objects + 1});
    Number(int num,           prop ss = {"TEMPORARY", __FILE__ , -1, num_of_objects + 1});
    Number(double num,        prop ss = {"TEMPORARY", __FILE__ , -1, num_of_objects + 1});
    Number(const char* str,   prop ss = {"TEMPORARY", __FILE__ , -4, num_of_objects + 1});
    Number(const Number& rhs, prop ss = {"TEMPORARY", __FILE__ , -5, num_of_objects + 1});
    Number(const Number&& rhs);

    ~Number();

    int getval() const { return val; }


    friend Number operator+(const Number& lhs, const Number& rhs);
    friend Number operator-(const Number& lhs, const Number& rhs);
    friend Number operator*(const Number& lhs, const Number& rhs);
    friend Number operator/(const Number& lhs, const Number& rhs);
    friend Number operator%(const Number& lhs, const Number& rhs);
    friend Number operator&(const Number& lhs, const Number& rhs);
    friend Number operator|(const Number& lhs, const Number& rhs);


    Number& operator+()                   { /* Log log(__PRETTY_FUNCTION__); log.printProp(*this); */ return *this; }
    Number  operator-()                   { /* Log log(__PRETTY_FUNCTION__); log.printProp(*this); */ return Number(-val); }
    Number& operator++()                  { /* Log log(__PRETTY_FUNCTION__); log.printProp(*this); */ val++; return *this; }
    Number  operator++(int)               { /* Log log(__PRETTY_FUNCTION__); log.printProp(*this); */ Number temp {*this}; ++(*this); return temp; }
    Number& operator--()                  { /* Log log(__PRETTY_FUNCTION__); log.printProp(*this); */ val--; return *this; }
    Number  operator--(int)               { /* Log log(__PRETTY_FUNCTION__); log.printProp(*this); */ Number temp {*this}; --(*this); return temp; }
    Number& operator= (const Number& rhs) { /* Log log(__PRETTY_FUNCTION__); log.printProp(*this); */ val = rhs.val; return *this; }

    bool operator==(const Number& rhs) { m_compare_number++; /* Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return val == rhs.val; }
    bool operator!=(const Number& rhs) { m_compare_number++; /* Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return !(*this == rhs); }
    bool operator> (const Number& rhs) { m_compare_number++; /* Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return val > rhs.val; }
    bool operator< (const Number& rhs) { m_compare_number++; /* Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return val < rhs.val; }
    bool operator>=(const Number& rhs) { m_compare_number++; /* Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return !(*this < rhs);}
    bool operator<=(const Number& rhs) { m_compare_number++; /* Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return !(*this > rhs);}

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
