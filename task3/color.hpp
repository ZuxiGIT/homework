#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

struct Color
{
    float r = 0;
    float g = 0;
    float b = 0;

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
    
    ~Color() = default;
    
    operator sf::Vector3f() const { return sf::Vector3f( r, g, b); } 
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