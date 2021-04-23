#include "objects.hpp"
#include "functions.inl"
#include <stdio.h>
#include <cmath>





Sphere::Sphere(sf::Vector3f center, float radius, Material properties, sf::Color color)
: 
Drawable(center, color, properties),
m_radius(radius)
{}

sf::Vector3f Sphere::ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const
{
    sf::Vector3f OC = origin - m_position;

    
    float k1 = dot(direction, direction);
    float k2 = 2 * dot(direction, OC);
    float k3 = dot(OC, OC) - m_radius * m_radius;
    
    float discr = k2 * k2 - 4 * k1 * k3;
    
    if(discr < 0) return sf::Vector3f(0, _INFINITY, _INFINITY);
    
    float t1 = (-k2 + sqrtf(discr)) / ( 2 * k1);
    float t2 = (-k2 - sqrtf(discr)) / ( 2 * k1);
    
    return sf::Vector3f(1, t1, t2);

    #if 0
    if( t2 > 1 && t1 > 1)
    {
        if(direction.x == 0 && direction.y == 0)
        {    
            fprintf(stderr, "OC (%f, %f, %f)\n", OC.x, OC.y, OC.z);
            fprintf(stderr, "Solutions: t1 = %f t2 = %f Direction (%f, %f, %f)   ", t1, t2, direction.x, direction.y, direction.z);
            fprintf(stderr, "Points: (%f, %f, %f) (%f, %f, %f)\n", (t1*direction).x , (t1*direction).y, (t1*direction).z, (t2*direction).x, (t2*direction).y, (t2*direction).z);
        }
        
        return true;
    }
    return false;
    #endif
}

Plane::Plane(sf::Vector3f plane_point, sf::Vector3f plane_normal, Material properties, sf::Color color)
:
Drawable(plane_point, color, properties),
m_plane_normal(plane_normal)
{}

sf::Vector3f Plane::ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const
{

    //fprintf(stderr, " (%f, %f, %f) - (%f, %f, %f) ", origin.x, origin.y, origin.z, m_position.x , m_plane_point.y, m_plane_point.z);
    float a = dot(origin - m_position, m_plane_normal);
    float b = dot(direction, m_plane_normal);
    //fprintf(stderr, " t = %f / %f\n", a , b);
    float t = - a / b;



    if( t >= _INFINITY)
    {
        fprintf(stderr, "Plane intersection< but t > _INFINITY * _INFINITY, t = %f\n", t);
        return sf::Vector3f(0, _INFINITY, _INFINITY);
    }



    //fprintf(stderr, "Plane intersection< but t (%f)< _INFINITY\n", t);
    return sf::Vector3f(1, t, t);
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

Light::Light(Type type, float intensity, sf::Vector3f dir_or_pos)
:
m_type(type),
m_intensity(intensity)
{
    if(m_type != Light::Type::AMBIENT)
        m_position = dir_or_pos;
}


LightManager::~LightManager()
{
    for(size_t i = 0; i < m_count; i ++)
        delete m_lights[i];
}

void LightManager::add(Light* obj)
{
    m_count++;
    m_lights.push_back(obj);
}