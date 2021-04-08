#pragma once
#include <string>
#include <fstream>
#include <sstream>

#define _ERROR(_str) fprintf(stderr, _str);
#define _CHAR(x)  std::to_string(x)


struct Obj2log
{
	virtual std::string createGraphNode()   const = 0;//  { return ""; }
	virtual std::string createHistoryEdge() const = 0;//{ return ""; }
    virtual std::string getGraphNodeInfo(std::string add_name = ""  )  const = 0;
    virtual int getNumber() const = 0;
    virtual ~Obj2log() {}
};

class Logger
{
    static std::string filepath;
    static unsigned int instances;
protected:
    static int m_shift;
    static std::ostringstream m_output;
    void shift(); 
public:
    Logger();
    explicit Logger(const std::string& filename);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    ~Logger();
};




class LogFunc : public Logger
{
protected:
    int previous_function = -1; 
    std::string previous_cluster_function_name = "";
    std::string previous_function_name = "";

    static std::string current_function_name;
    static std::string current_cluster_function_name; 
    static int curr_function;
    static unsigned int instances;
    static int function_counter;
public:
    LogFunc(const std::string& funcname);
  
    
    LogFunc(const LogFunc&) = delete;
    LogFunc& operator=(const LogFunc&) = delete;

    ~LogFunc();
};


class LogObj : public Logger
{
    enum NodeType {DOUBLE_NODE, SINGLE_NODE, NOT_DEFINED};
    static NodeType last_node;
    static int last_double_node;
	static int last_Obj;
public:
    LogObj(const Obj2log& obj);

    LogObj(const Obj2log& lhs, const Obj2log& rhs);

    static std::string getNodeInfo(const Obj2log& obj, std::string add_name = "");

    LogObj(const LogObj&) = delete;
    LogObj& operator=(const LogObj&) = delete;

    ~LogObj();

};
