#include "logger.hpp"
#include <iostream>
#include <cassert>

std::ostringstream Logger::m_output = {};
std::string Logger::filepath = "";
int LogObj::last_Obj = 0;
int LogObj::last_double_node = 0;
LogObj::NodeType LogObj::last_node = NOT_DEFINED;
std::string LogFunc::current_function_name = "";
std::string LogFunc::current_cluster_function_name = "";
int LogFunc::curr_function = -1;
int LogFunc::function_counter = 0;
unsigned int Logger::instances = 0;
unsigned int LogFunc::instances = 0;
int Logger::m_shift  = 0;

Logger::Logger(const std::string& filename)
{
    _ERROR("LOGGER(std::string)\n")
    
    instances++;
    filepath = filename;
    
    if(!m_output.good())
    {
        _ERROR("SOMETHING WRONG")
        fflush(0);
        exit(1);
    }
    
    m_output << "digraph test{\n";

    m_shift++;
    shift();
    m_output << "rankdir=TB;\n";
}

Logger::Logger()
{
    instances++;
    _ERROR("LOGGER()\n")
}

void Logger::shift()
{
    for(int i = 0; i < m_shift; ++i)
        m_output << "\t";
}


Logger::~Logger()
{
	_ERROR("~Logger()\n")
    
    instances--;
    
    if(!instances)
    {
        m_output << "}";
        FILE* fp = fopen(filepath.c_str(), "w");
        
        if(!fp)
        {
            _ERROR("ERROR::FILE_NOT_OPENED\n")
            return;
        }

        fprintf(fp, m_output.str().c_str());
    }
}

LogFunc::LogFunc(const std::string& funcname)
{
    curr_function = function_counter;
    
    if(!current_function_name.empty())
        previous_function_name = current_function_name;

    current_function_name = funcname;

    if(!current_cluster_function_name.empty())
        previous_cluster_function_name = current_cluster_function_name + "_close"; 
    
    function_counter++;
	
    current_cluster_function_name = "cluster_" + _CHAR(function_counter);
    
    if(instances++)
    {   
        previous_function = curr_function;
        m_shift--;
        shift();
        m_output << "}\n";

    }

	_ERROR("LOGFUNC()\n")
	
	shift();
    m_output << "subgraph cluster_" << _CHAR(function_counter) << "{\n"; 
    
    m_shift++;

    shift();
    m_output << "color=lightgrey;\n";
    shift();
    m_output << "style=filled;\n";
    shift();
    m_output << "label= \""<< funcname << "\";\n";

}

LogFunc::~LogFunc()
{
	m_shift--;
	shift();
	m_output << "}\n";

    if(--instances)
    {
        shift();
        m_output << "subgraph "<< previous_cluster_function_name << "{\n";
        m_shift++;


        shift();
        m_output << "color=lightgrey;\n";
        shift();
        m_output << "style=filled;\n";
        shift();
        m_output << "label= \""<< previous_function_name << "\";\n";

        curr_function = previous_function;
    }
    _ERROR("~LOGFUNC()\n");
}

LogObj::LogObj(const Obj2log& obj)
{
	_ERROR("LogObj(const Obj2log& obj)\n")	
	std::string Node = obj.createGraphNode();
	
	if(!Node.empty())
	{
		shift();
		m_output << Node;
	}

	std::string edge = obj.createHistoryEdge();
	
	if(!edge.empty())
	{
		shift();
		m_output << edge;
	}

    if(last_Obj && last_node == SINGLE_NODE)
    {
        shift();
        m_output << "node_" << last_Obj << "->" << "node_" << obj.getNumber() \
        << "[color=\"red\"];\n"; 
    }
    else if(last_double_node && last_node == DOUBLE_NODE)
    {
        shift();
        m_output << "Dnode_" << last_double_node << "->" << "node_" << obj.getNumber() \
        << "[color=\"red\"];\n"; 
    }

    last_Obj = obj.getNumber();
    last_node = SINGLE_NODE;
}


LogObj::LogObj(const Obj2log& lhs, const Obj2log& rhs)
{
    _ERROR("LogObj(const Obj2log& lhs, const Obj2log& rhs)\n")

    last_double_node++;
    std::string left  = getNodeInfo(lhs, "left");
    std::string right = getNodeInfo(rhs, "right");

    std::string res = left + " | " + right;

    if(last_Obj && last_node == SINGLE_NODE)
    {
        shift();
        m_output << "node_" << last_Obj << "->" << "Dnode_" << last_double_node \
        << "[color=\"red\"];\n"; 
    }
    else if(last_double_node && last_node == DOUBLE_NODE)
    {
        shift();
        m_output << "Dnode_" << last_double_node - 1<< "->" << "Dnode_" << last_double_node \
        << "[color=\"red\"];\n"; 
    }

    shift();
    m_output << "Dnode_" << _CHAR(last_double_node) << "[shape=record,label="\
    << "\"" << res << "\"];\n";

    std::string left_edge = "node_" + _CHAR(lhs.getNumber()) + "->" + \
    "Dnode_" + _CHAR(last_double_node) + ":<name_left> [color=\"black\", style=\"dashed\"];\n"; 
    
    std::string right_edge = "node_" + _CHAR(rhs.getNumber()) + "->" + \
    "Dnode_" + _CHAR(last_double_node) + ":<name_right> [color=\"black\", style=\"dashed\"];\n";

    shift();
    m_output << left_edge;
    shift();
    m_output << right_edge;

    last_node = DOUBLE_NODE;

}

std::string LogObj::getNodeInfo(const Obj2log& obj, std::string add_name)
{
    return obj.getGraphNodeInfo(add_name);
}

LogObj::~LogObj()
{
	_ERROR("~LogObj()\n")
}