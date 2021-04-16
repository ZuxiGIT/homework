#include "objects.hpp"
#include <stdio.h>
#include <cmath>

inline float dot(const sf::Vector3f& lhs, const sf::Vector3f& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}


Sphere::Sphere(sf::Vector3f center, float radius, sf::Color color)
: 
m_center(center),
m_radius(radius),
m_color(color)
{}

bool Sphere::ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const
{
    sf::Vector3f OC = origin - m_center;

    
    float k1 = dot(direction, direction);
    float k2 = 2 * dot(direction, OC);
    float k3 = dot(OC, OC) - m_radius * m_radius;
    
    float discr = k2 * k2 - 4 * k1 * k3;
    
    if(discr < 0) return false;
    
    float t1 = (-k2 + sqrtf(discr)) / ( 2 * k1);
    float t2 = (-k2 - sqrtf(discr)) / ( 2 * k1);
    
    
    if( t2 > 1 && t1 > 1)
    {
        #if 0
        if(direction.x == 0 && direction.y == 0)
        {    
            fprintf(stderr, "OC (%f, %f, %f)\n", OC.x, OC.y, OC.z);
            fprintf(stderr, "Solutions: t1 = %f t2 = %f Direction (%f, %f, %f)   ", t1, t2, direction.x, direction.y, direction.z);
            fprintf(stderr, "Points: (%f, %f, %f) (%f, %f, %f)\n", (t1*direction).x , (t1*direction).y, (t1*direction).z, (t2*direction).x, (t2*direction).y, (t2*direction).z);
        }
        #endif
        return true;
    }
    return false;
}



void ObjectManager::add(Drawable* obj)
{
    m_count++;
    m_objects.push_back(obj);
}

ObjectManager::~ObjectManager()
{
    for(size_t i = 0; i < m_count; i++)
        delete m_objects[i];
}