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

inline sf::Vector3f normalize(const sf::Vector3f& obj)
{
    float len = length(obj);
    return sf::Vector3f(obj / len);
}
