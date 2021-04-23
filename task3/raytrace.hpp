#pragma once
#include "color.hpp"
#include "ray.hpp"

struct Raytrace
{
    static Color BACKGROUND;
    static Color ray_cast(Ray& ray, float t_min, float t_max, const ObjectManager &objects, const LightManager &lights);
    static float  ComputeLighting(Ray &ray, const sf::Vector3f &normal, Material prop, const ObjectManager &objects, const LightManager &lights);
};
