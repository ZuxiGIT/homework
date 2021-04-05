#include "Log.hpp"
#include <stdio.h>
//#include <windows.h>
//#include <wincon.h>

int Log::m_shift = 0;

Log::Log(const char* function, COL background)
{
    shift(); 
    call(function, background);
    shift();
    printf("{\n"); 
    m_shift++;
}

Log::~Log()
{
    m_shift--;
    shift();
    printf("}\n");
}

void Log::shift()
{
    for (int i = 0; i < m_shift; i++) 
        printf("\t");
}


void Log::call(const char* function, COL background)
{
    COL oldColour = GetConsoleColour();

    SetConsoleColour(WHITE, background);
    fprintf(stderr, "%s wad called\n", function);

    SetConsoleColour(oldColour & 0xFF, COL((oldColour & 0xFF00) >> 4));
}

void Log::copy(const Number& lhs, const Number& rhs)
{
    COL oldColour = GetConsoleColour();
    
    SetConsoleColour(YELLOW, BLACK);
    shift();
    fprintf(stderr, "copy object [");
    SetConsoleColour(LIGHTBLUE, BLACK);
    fprintf(stderr,  "%p", &rhs);
    SetConsoleColour(YELLOW, BLACK);
    fprintf(stderr, "] ---> [");
    SetConsoleColour(GREEN, BLACK);
    fprintf(stderr, "%p", &lhs);
    SetConsoleColour(YELLOW, BLACK);
    fprintf(stderr, "]\n");
    shift();
    fprintf(stderr, "value: %d\n", rhs.getval());

    SetConsoleColour(oldColour & 0xFF, COL((oldColour & 0xFF00) >> 4));
}

void Log::printProp(const Number& ss)
{
    COL oldColour = GetConsoleColour();

    shift();
    SetConsoleColour(WHITE, BLUE);
    fprintf(stderr,  "----Variable properties----\n");
    SetConsoleColour(CYAN, BLACK);
    shift();
    fprintf(stderr, "\tname of var:");
    SetConsoleColour(LIGHTRED, BLACK);
    fprintf(stderr, " %s\n", ss.info.name);
    SetConsoleColour(CYAN, BLACK);
    shift();
    fprintf(stderr, "\tfilename: %s\n", ss.info.file);
    shift();
    fprintf(stderr, "\tline number: %d\n", ss.info.line);
    shift();
    fprintf(stderr, "\tconstructor: %s\n", ss.info.constructor);
    shift();
    fprintf(stderr, "\tnumber: %d\n", ss.info.number);
    shift();
    fprintf(stderr, "\tvalue: ");
    SetConsoleColour(LIGHTRED, BLACK);
    fprintf(stderr, "%d\n", ss.val);
    shift();
    SetConsoleColour(CYAN, BLACK);
    fprintf(stderr, "\taddres of number:");
    SetConsoleColour(LIGHTRED, BLACK);
    fprintf(stderr, " [%p]\n", &ss);

    SetConsoleColour(oldColour & 0xFF, COL((oldColour & 0xFF00) >> 4));
}

void Log::summary()
{
    COL oldColour = GetConsoleColour();

    shift();
    SetConsoleColour(BLACK, YELLOW);
    fprintf(stderr, "number of objects:  %d\n", Number::num_of_objects);
    SetConsoleColour(BLACK, BLACK);
    shift();
    SetConsoleColour(BLACK, YELLOW);
    fprintf(stderr, "maximum of objects: %d\n", Number::max_size);

    SetConsoleColour(oldColour & 0xFF, COL((oldColour & 0xFF00) >> 4));
}

void Log::function(const Number& lhs, const Number& rhs)
{
    COL oldColour = GetConsoleColour();

    shift();
    SetConsoleColour(CYAN, BLACK);
    fprintf(stderr, "left  object addres");
    SetConsoleColour(LIGHTRED, BLACK);
    fprintf(stderr, " [%p]", &lhs);
    SetConsoleColour(CYAN, BLACK);
    fprintf(stderr, " value: %d;\n", lhs.getval());
    shift();
    fprintf(stderr, "right object addres");
    SetConsoleColour(LIGHTRED, BLACK);
    fprintf(stderr, " [%p]", &rhs);
    SetConsoleColour(CYAN, BLACK);
    fprintf(stderr, " value: %d;\n", rhs.getval());

    SetConsoleColour(oldColour & 0xFF, COL((oldColour & 0xFF00) >> 4));
}

// COL SetConsoleColour(COL foreground, COL background)
// {
//     COL oldColour = GetConsoleColour();
    
//     if(!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)(foreground | background << 4)))
//     {
//         fprintf(stderr, "ERROR::SETCONSOLETEXTATTRIBURE\n");
//         exit(1);
//     }

//     return oldColour;
// }

// COL GetConsoleColour()
// {
//     CONSOLE_SCREEN_BUFFER_INFO config = {};

//     if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &config))
//     {
//         fprintf(stderr, "ERROR::GETCONSOLESCREENBUFFERINFO\n");
//         exit(1);
//     }

//     return (COL)(config.wAttributes & 0xFF);
// }