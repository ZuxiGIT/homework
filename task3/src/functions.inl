// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#pragma once
#include <SFML/System/Vector3.hpp>
#include <cmath>

inline float dot(const sf::Vector3f& lhs, const sf::Vector3f& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline float length(const sf::Vector3f& obj)
{
    return sqrtf(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z);
}

inline sf::Vector3f cross(sf::Vector3f a, sf::Vector3f b)
{
    float i = 0, j = 0, k = 0;
    i = a.y*b.z - a.z*b.y;
    j = a.z*b.x - a.x*b.z;
    k = a.x*b.y - a.y*b.x;
    return sf::Vector3f(i, j, k);
}
inline sf::Vector3f normalize(const sf::Vector3f& obj)
{
    float len = length(obj);
    return sf::Vector3f(obj / len);
}

inline sf::Vector2f rot(double a)
{
    float s = static_cast<float>(sin(a));
    float c = static_cast<float>(cos(a));
    return sf::Vector2f(c, s);
}