#pragma once
#include "logger.hpp"

#define NUMBER_(var,...) Number var (__VA_ARGS__ __VA_OPT__(,) {#var , __FILE__, __LINE__})

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

    std::string getGraphNode(int value) const ;
    std::string getGraphEdge() const;
    std::string getGraphNodeInfo(int value, std::string add_name = "") const;
    int getNumber() const;
};


class Number : public Obj2log
{
    int val;
    prop info;
public:
    static int num_of_objects;
    static int max_size;

    virtual std::string createGraphNode()   const override   { return info.getGraphNode(val); } 
    virtual std::string createHistoryEdge() const override   { return info.getGraphEdge(); }
    virtual std::string getGraphNodeInfo(std::string add_name = "")  const override   { return info.getGraphNodeInfo(val, add_name); }
    virtual int getNumber() const override { return info.getNumber(); }
    
    //static void printStat();

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

    bool operator==(const Number& rhs) { /* m_compare_number++;  Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return val == rhs.val; }
    bool operator!=(const Number& rhs) { /* m_compare_number++;  Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return !(*this == rhs); }
    bool operator> (const Number& rhs) { /* m_compare_number++;  Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return val > rhs.val; }
    bool operator< (const Number& rhs) { /* m_compare_number++;  Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return val < rhs.val; }
    bool operator>=(const Number& rhs) { /* m_compare_number++;  Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return !(*this < rhs);}
    bool operator<=(const Number& rhs) { /* m_compare_number++;  Log log(__PRETTY_FUNCTION__); log.function(*this, rhs); */ return !(*this > rhs);}

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
