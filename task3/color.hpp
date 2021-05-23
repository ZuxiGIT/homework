#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>
#include <stdio.h>
typedef unsigned char BYTE;


//---------------------------deprecated---------------------------
struct Color
{
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;

    Color() = default;
    
    Color( float R, float G, float B){ 
                                        r = R > 1 ? 1 : R;
                                        g = G > 1 ? 1 : G;
                                        b = B > 1 ? 1 : B;
                                     }
    
    Color( const sf::Vector3f& that){  
                                        r = that.x  > 1 ? 1 : that.x;
                                        g = that.y  > 1 ? 1 : that.y;
                                        b = that.z  > 1 ? 1 : that.z;
                                    }
    Color( const sf::Color& that) { r = that.r / 255; g = that.g / 255; b = that.b / 255; }
    
    Color(const Color& obj) { r = obj.r; g = obj.g; b = obj.b; }
    
    Color& operator=(const Color& obj) { r = obj.r; g = obj.g; b = obj.b; return *this; }
    Color operator+(const Color& obj)  { return Color(  r + obj.r > 1 ? 1 : r + obj.r, 
                                                        g + obj.g > 1 ? 1 : g + obj.g,
                                                        b + obj.b > 1 ? 1 : b + obj.b); }



    ~Color() = default;
    
    operator sf::Vector3f() const { return sf::Vector3f( r, g, b); }
    //operator const sf::Vector3f() const { return sf::Vector3f( r, g, b); }

    operator sf::Color() const { return sf::Color(  static_cast<unsigned char>(r * 255),
                                                    static_cast<unsigned char>(g * 255),
                                                    static_cast<unsigned char>(b * 255)); }

    // operator const sf::Color() const { return sf::Color(static_cast<unsigned char>(r * 255),
    //                                                     static_cast<unsigned char>(g * 255),
    //                                                     static_cast<unsigned char>(b * 255)); }

};

template<typename T>
inline Color operator*(T num, const Color& rhs)
{
    return Color (rhs.r * num, rhs.g * num, rhs.b * num); 
}
template<typename T>
inline Color operator*(const Color& rhs, T num)
{
    return num * rhs;
}
template<typename T>
inline Color operator/(const Color& rhs, T num)
{
    return Color( rhs.r / num, rhs.g / num, rhs.b / num);
}
//-----------------above will be removed soon------------------

struct RGB
{
    BYTE r = 0;
    BYTE g = 0;
    BYTE b = 0;

    RGB() = default;

    RGB(BYTE R, BYTE G, BYTE B) : r(R), g(G), b(B) {}

    RGB(const RGB& that) : r(that.r), g(that.g), b(that.b) {}
    
    //RGB(const sf::Color& that) : r(that.r), g(that.g), b(that.b) {}
    RGB(const Color& that) :    r(static_cast<BYTE>(that.r * 255)),
                                g(static_cast<BYTE>(that.g * 255)),
                                b(static_cast<BYTE>(that.b * 255)) {}
    
    RGB& operator=(const RGB& obj) {  r = obj.r; g = obj.g; b = obj.b; return *this; }

    RGB operator+(const RGB& obj)  { return RGB(    static_cast<BYTE>(r + obj.r),       // r + obj.r > 1 ? 1 : r + obj.r 
                                                    static_cast<BYTE>(g + obj.g),       // g + obj.g > 1 ? 1 : g + obj.g
                                                    static_cast<BYTE>(b + obj.b)); }    // b + obj.b > 1 ? 1 : b + obj.b
    
    operator sf::Color() const { return sf::Color(r, g, b); }
    
    ~RGB() = default;
};

template<typename T>
inline RGB operator*(T num, const RGB& rhs)
{
    return Color (rhs.r * num, rhs.g * num, rhs.b * num); 
}
template<typename T>
inline RGB operator*(const RGB& rhs, T num)
{
    return num * rhs;
}
template<typename T>
inline RGB operator/(const RGB& rhs, T num)
{
    return RGB( rhs.r / num, rhs.g / num, rhs.b / num);
}

struct HSL
{
    BYTE h = 0;
    BYTE s = 0;
    BYTE l = 0;

    HSL() = default;

    HSL(BYTE H, BYTE S, BYTE L) : h(H), s(S), l(L) {}

    HSL(const HSL& that) : h(that.h), s(that.s), l(that.l) {}

    HSL& operator=(const HSL& obj) {  h = obj.h; s = obj.s; l = obj.l; return *this; }

    HSL operator+(const HSL& obj)  { return HSL(    static_cast<BYTE>(h + obj.h), 
                                                    static_cast<BYTE>(s + obj.s),
                                                    static_cast<BYTE>(l + obj.l)); }

    ~HSL() = default;
};

inline HSL RGB2HSL(const RGB& rgb)
{
    float   max = std::max(std::max(rgb.r, rgb.b), rgb.b) / 255.f,
            min = std::min(std::min(rgb.r, rgb.b), rgb.b) / 255.f,
            dm = max - min,
            sm = max + min;

    const float _EPS = static_cast<float>(1e-3);

    float   fr = rgb.r / 255.f,
            fg = rgb.g / 255.f,
            fb = rgb.b / 255.f;

    float   h = 0,
            s = (fabs(dm) < _EPS) ? 0 : static_cast<float>(dm / (1.f - fabs(1 - sm))),
            l = sm / 2.f;

    if(fabs(dm) > _EPS)
    {
        float   dr = fr / dm,
                dg = fg / dm,
                db = fb / dm;

        if(fabs(fr - max) < _EPS)   h = dg - db;
        if(fabs(fg - max) < _EPS)   h = 2 + db - dr;
        if(fabs(fb - max) < _EPS)   h = 4 + dr - dg;
    }
    h = (h < 0) ? h * 60 + 360 : h * 60;
    #define debug(x) {fprintf(stderr, "return value ("#x") is: %f\n", x); fflush(0);}
    debug(h)
    debug(s)
    debug(l)
    
    

    return HSL( static_cast<BYTE>(std::round(h / 360 * 255)),
                static_cast<BYTE>(std::round(s * 255)),
                static_cast<BYTE>(std::round(l * 255)));

}

inline RGB HSL2RGB(const HSL& hsl)
{
    if (hsl.s == 0) return RGB (0, 0, 0);
    struct CALC
    {
        #define RETURN(x) {fprintf(stderr, "return value ("#x") is: %f\n", x); fflush(0); return x;}
        
        static float calc(float component, float Q, float P)
        {
            component += (component < 0) ? 1.f : 0.f;
            component -= (component > 1) ? 1.f : 0.f;
            
            fprintf(stderr, "componentent %f\n", component);
            
            if(6*component < 1)                     RETURN(P + ((Q - P) * 6.f * component)) 
            if(1 <= 6*component && 6*component < 3) RETURN(Q)
            if(3 <= 6*component && 6*component < 4) RETURN(P + ((Q - P) * ( 2.f / 3.f - component) * 6.f))
            RETURN(P)
        }
    };
    float H = hsl.h / 255.f;
    float S = hsl.s / 255.f;
    float L = hsl.l / 255.f;


    float Q = (L < 0.5f) ?  L * (1.f + S) : L + S - (L * S);
    float P = 2.f * L - Q;
    float Hk = H;
    float Tr = Hk + 1.f / 3.f;
    float Tg = Hk;
    float Tb = Hk - 1.f / 3.f;
    fprintf(stderr, "Tr, Tg, Tb, Hk, P, Q : %f, %f, %f, %f, %f, %f\n", Tr, Tg, Tb, Hk, P, Q);
    
    return RGB( static_cast<BYTE>(std::round(255 * CALC::calc(Tr, Q, P))),
                static_cast<BYTE>(std::round(255 * CALC::calc(Tg, Q, P))),
                static_cast<BYTE>(std::round(255 * CALC::calc(Tb, Q, P)))); 
}