#pragma once
#include "color.hpp"
#include "objects.hpp"

class Ray
{

public:

static Color ray_cast(const sf::Vector3f& origin, const sf::Vector3f& direction, const ObjectManager& objects, const LightManager& lights);
};


