#pragma once
#include "color.hpp"
#include "objects.hpp"
#include "noncopyable.hpp"
#include "functions.inl"

struct Ray
{

    static int recursion_depth;
    sf::Vector3f m_origin = {};
    sf::Vector3f m_direction = {};
    int m_generation = 0;

    void reflect(const sf::Vector3f& normal) 
    {   
        m_direction = 2.f * normal * dot(normal, m_direction) - m_direction; 
    } 


    sf::Vector3f getPoint(float t) const { return m_origin + t * m_direction; } // ???????????????????????????????
    sf::Vector3f getPoint(float t) { return m_origin + t * m_direction; }
    
    sf::Vector2f closestIntersection(float t_min, float t_max, const ObjectManager& objects) const;
    
    explicit Ray(const sf::Vector3f& origin, const sf::Vector3f& direction);
};





