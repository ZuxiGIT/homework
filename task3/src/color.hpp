#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>
#include <stdio.h>
typedef unsigned char BYTE;

struct RGB;
struct HSL;
RGB HSL2RGB(const HSL&);
HSL RGB2HSL(const RGB&);


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
    float h = 0;
    float s = 0;
    float l = 0;

    HSL() = default;

    HSL(float H, float S, float L) : h(H), s(S), l(L) {}

    HSL(const HSL& that) : h(that.h), s(that.s), l(that.l) {}
/*
    HSL& operator=(const HSL& obj) {  h = obj.h; s = obj.s; l = obj.l; return *this; }

    HSL operator+(const HSL& obj)  { return HSL(    static_cast<BYTE>(h + obj.h), 
                                                    static_cast<BYTE>(s + obj.s),
                                                    static_cast<BYTE>(l + obj.l)); }
*/
    ~HSL() = default;
};