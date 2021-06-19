// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ray.hpp"
#include "functions.inl"


int Ray::recursion_depth = 1;

Ray::Ray(const sf::Vector3f &origin, const sf::Vector3f &direction)
: 
m_origin(origin),
m_direction(direction),
m_generation(Ray::recursion_depth)
{}

sf::Vector2f Ray::closestIntersection(float t_min, float t_max, const ObjectManager &objects) const
{
    float min_dist = _INFINITY;

    union
    {
        float fl;
        const Drawable *ptr;
    } closest_obj;

    closest_obj.ptr = nullptr;

    // if(objects.size() != 5)
        // return sf::Vector2f(min_dist, closest_obj.fl);
    
    
    for (size_t i = 0; i < objects.size(); i++)
    {
        sf::Vector3f solutions = objects[i].ray_intersect(m_origin, m_direction);
        
        if (solutions.x < _EPS)
            continue;

        float t1 = solutions.y;
        float t2 = solutions.z;

        if (t1 < min_dist && t1 > t_min && t1 < t_max)
        {
            min_dist = t1;
            closest_obj.ptr = &objects[i];
        }

        if (t2 < min_dist && t2 > t_min && t2 < t_max)
        {
            min_dist = t2;
            closest_obj.ptr = &objects[i];
        }
    }
    
    return sf::Vector2f(min_dist, closest_obj.fl);
}