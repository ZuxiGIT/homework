#include "objects.hpp"

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
    float k1 = dot(direction, direction);
    sf::Vector3f OC = m_center - origin;
    float k2 = 2 * dot(direction, OC);
    float k3 = dot(OC, OC) - m_radius * m_radius;
    float discr = k2 * k2 - 4 * k1 * k3;
    if(discr < 0) return false;
    if(discr == 0) if( -k2 / (2*k1) > 1) return true;
    return true;


}