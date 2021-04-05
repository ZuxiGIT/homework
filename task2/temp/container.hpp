#pragma once
#include <string>
//#include "Number.hpp"
//#include "Log.hpp"


typedef enum{
    ARRAY,
    VECTOR,
    NOT_DEFINED
} TYPE;

struct params
{
    const char* name = nullptr;
    const char* file = nullptr;
    int line = -1;
    int number = -1;
    TYPE container_type = NOT_DEFINED;
    const char* type = nullptr;  
    const char* constructor = nullptr;

    std::string getGraphNode() const;
};

std::string params::getGraphNode() const 
{
    std::string res = {};
    return res;
}


class Container
{

    params m_info;
public:

    friend void PrintVector();

    static int num_of_objects;
    static int max_size;
    //friend void Log::summary();


    Container(params ss) : m_info(ss) { num_of_objects++; max_size++; }
    virtual ~Container(){ num_of_objects--; }
    virtual int getSize() const { return 5; }
};

