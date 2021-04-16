#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <typeinfo>
#include <utility>

template<int N>
struct Print{};



template<int N>
struct factorial
{
    static const int value = /*(Print< N * factorial<N-1>::value>::please,*/ N * factorial<N-1>::value ;
};

template<>
struct factorial<0>
{
    static const int value = /*(Print<1>::please,*/ 1/*)*/;
    //int fake = Print<value>::mew;
};


template<int N>
struct fibbonachi
{
    enum{ value = fibbonachi<N-2>::value + fibbonachi<N-1>::value };
};

template<>
struct fibbonachi<1>
{
    enum{ value = 1};
};

template<>
struct fibbonachi<0>
{
    enum{ value = 1};
};

template<int N, int P = N-1>
struct isPrime
{
    enum{ value = !(N % P == 0) && isPrime<N, P-1>::value };
};

template<int N>
struct isPrime<N, 2>
{
    enum{ value = !(N % 2 == 0)};
};




template <int a, int b>
struct euclid
{
    static const int value = euclid<b, a%b>::value;
};

template<int b>
struct euclid<0, b>
{
    static const int value = b;
};

template<int b>
struct euclid<1, b>
{
    static const int value = 1;
};

template<int a>
struct euclid<a, 0>
{
    static const int value = a;
};

template<int a>
struct euclid<a, 1>
{
    static const int value = 1;
};


template<typename T, size_t counter>
struct MatSum
{
    static void Mat_Sum(void* mat1, void* mat2)
    {
        *(T*)mat1 += *(T*)mat2;
        MatSum<T, counter - 1>::Mat_Sum((T*)mat1 + 1, (T*)mat2 + 1);
    }
};

template<typename T>
struct MatSum<T, 0>
{
    static void Mat_Sum(void* mat1, void* mat2)
    {
        *(T*)mat1 += *(T*)mat2;
    }
};


template<typename T, size_t row, size_t col>
struct matSum
{
    static void mat_Sum(void* mat1, void* mat2)
    {
        MatSum<T, row*col - 1>::Mat_Sum(reinterpret_cast<void*>(mat1), reinterpret_cast<void*>(mat2));
    }
};








template<typename... Args>
struct NumOfArgs;

template<>
struct NumOfArgs<>
{
    enum{ value = 0};
};

template<typename T, typename... Args>
struct NumOfArgs<T, Args...>
{
    enum{ value = 1 + NumOfArgs<Args...>::value };
};

inline void Printf(const char* s)
{
    std::cout << s;
}

template<typename T, typename... Args>
void Printf(const char* s, T value, Args ... args)
{
    int num = NumOfArgs<Args...>::value + 1;
    std::cout <<"\n------->" << num << "and " << sizeof...(Args) <<"<-------\n";

    while(*s)
    {
        if(*s == '#')
        {
            std::cout << value;

            s++;

            Printf(s, args...);

            break;
        }
        putchar(*s++);
    }
}

inline void MyPrintf(const char* s, ...)
{
    va_list vl;
    va_start(vl, s);
    while(char c = *s++)
    {
        if(c == '#')
        {
            switch (*s)
            {
                case 'l':
                {
                    long arg = va_arg(vl, long);
                    printf("%ld", arg);
                    break;
                }
                case 'u':
                {
                    unsigned int arg = va_arg(vl, unsigned int);
                    printf("%u", arg);
                    break;
                }
                case 'c':
                case 'i':
                {
                    int arg = va_arg(vl, int);
                    printf("%d", arg);
                    break;
                }
                case 'f':
                case 'd':
                {
                    double arg = va_arg(vl, double);
                    printf("%lf", arg);
                    break;
                }
                case 's':
                {
                    const char* arg = va_arg(vl, char*);
                    printf("%s", arg);
                    break;
                }
                case 'p':
                {
                    void* arg = va_arg(vl, void*);
                    printf("%p", arg);
                    break;
                }
                default:
                    break;
            }
            s++;
            continue;
        }
        putchar(c);
    }
    va_end(vl);
}




template <typename T>
struct has_myFunc
{
    static int detect(...);   // fake
    template<typename U> static decltype(std::declval<U>().myFunc(42)) detect (const U&); // detector

    static constexpr bool value = std::is_same<void, decltype(detect(std::declval<T>()))>::value;
};

// int fac = Print<factorial<10>::value>::mew;
// int fib = Print<fibbonachi<5>::value>::mew;
// int isP = Print<isPrime<6>::value>::mew;
// int euc = Print<euclid<30, 12>::value>::mew;
inline void PrintFError(const char * format, ...) 
{
    char buffer[256] = {};
    va_list args;
    va_start(args, format);
    vsprintf(buffer,format, args);
    perror(buffer);
    va_end(args);
}


class myClass
{
    myClass(long ff) { printf("myClass(long) CONSTR\n"); } ;
    myClass() { printf("DEFAULT CONSTR\n"); };
public:
    void myFunc(int x) { return;} 
};


int main()
{ 
    //Print<has_myFunc<myClass>::value>::meow;
    MyPrintf("test Int #i Double #d String #s anothwe texxt afteer\n", 12, 3.14, "STRING_TEXTX");
    // int a[2][2] = {{1, 2}, {3, 4}};
    // int b[2][2] = {{5, 6}, {7, 8}};

    // std::cout <<typeid(long long).name();
    // Printf("absdb # asfaf # # #", 12, "hello", "from", 12343);
    // matSum<int, 2, 2>::mat_Sum(a, b);
    
    // for(int j = 0; j < 2;j ++)
    // {       
    //     for(int i = 0; i < 2;i ++)
    //         printf("%2d ", a[j][i]);
    //     printf("\n");
    // }

    getchar();
    
    return 0;
}
