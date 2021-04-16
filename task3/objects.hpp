#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


#if 0
class Object
{

};

#endif

class Sphere 
{
    sf::Vector3f m_center = {};
    float m_radius = 0;
    sf::Color m_color = {};

    public:

    explicit Sphere(sf::Vector3f position, float radius, sf::Color color = sf::Color::Green);

    Sphere(const Sphere&) = delete;
    Sphere& operator= (const Sphere&) = delete;

    bool ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const;

};