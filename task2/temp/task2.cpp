#include <logger.hpp>
//#include <iostream>
//#include <numeric>
//#include <template.hpp>
// #define ARRAY_(var,...)  ARRAY var  (__VA_ARGS__ __VA_OPT__(,) {#var , __FILE__, __LINE__, enum::ARRAY})
// #define VECTOR_(var, type, ...) Vector var<type> (__VA_ARGS__ __VA_OPT__(,) {#var , __FILE__, __LINE__, enum::VECTOR, #type})




// int main(void)
// {
//     {
//         //Array a(10);
//         Vector<int> b(10);
//         b[3] = b[3] + 56;
//         Vector<int> c(6);
//         c = b;
//         c[3] = c[3] + 56;

//         for (int i = 0; i < b.getSize(); i ++)
//             fprintf(stderr, "%d ", b[i]);

//     }
//     getchar();
//     return 0;
// }
//Logger logger{"myprog.dot"};


// LogFunc lglglg{};
// int main()
// {
//     double a[3] = {1.23, 2.34, 3.45};
//     double res = std::accumulate(a, a+3, 0.);
//     std::cout << res<<std::endl;
//     getchar();
//     return 0;
// }