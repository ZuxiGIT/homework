#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "color.hpp"
#include <vector>

const float _INFINITY = static_cast<float>(0xDEADBEEFDEADBEEF);
const float _EPS = static_cast<float>(1e-3);




struct Material
{
    int specular = 1;
};

struct  Drawable
{
    sf::Vector3f m_position = {};
    Material m_properties = {};
    Color m_color = {};

    Drawable(sf::Vector3f position, sf::Color color, Material properties) : m_position(position), m_properties(properties), m_color(color) {}

    Drawable(const Drawable&) = delete;
    Drawable& operator=(const Drawable&) = delete;

    virtual sf::Vector3f ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const = 0;
    virtual sf::Vector3f getNormal(const sf::Vector3f& point) const = 0;
    sf::Vector3f getColor() const  { return m_color; }
    sf::Vector3f getPosition() const { return m_position; }

    virtual ~Drawable() {}

};



class Sphere : public Drawable
{
    float m_radius = 0;

public:

    explicit Sphere(sf::Vector3f position, float radius, Material properties, sf::Color color = sf::Color::Green);
    Sphere() = default;

    virtual sf::Vector3f ray_intersect (const sf::Vector3f& origin, const sf::Vector3f& direction) const override;
    virtual sf::Vector3f getNormal(const sf::Vector3f& point) const override { return point - m_position;}
    virtual ~Sphere() override {}
};


class Plane : public Drawable
{
    sf::Vector3f m_plane_normal = {};
public:

    explicit Plane(sf::Vector3f plane_point, sf::Vector3f plane_normal, Material properties, sf::Color color = sf::Color::Magenta);
    
    virtual sf::Vector3f ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const override;
    virtual sf::Vector3f getNormal(const sf::Vector3f&) const override { return m_plane_normal; }
    virtual ~Plane() override {}
};

class ObjectManager
{
    unsigned int m_count = 0;
    std::vector<Drawable*> m_objects = {};

    ObjectManager() = default;
    ~ObjectManager();

public:
    static ObjectManager& createManager()
    { static ObjectManager instance; return instance; }

    ObjectManager(ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;

    unsigned int size() const { return m_count; }
    
    Drawable& operator[](size_t index) { return *m_objects[index]; }
    const Drawable& operator[] (size_t index) const  { return *m_objects[index]; }

    void add(Drawable* obj);
};


struct Light
{

    enum class Type
    {
        AMBIENT,
        POINT,
        DIRECTIONAL
    };

    Type m_type;
    union
    {
        sf::Vector3f m_direction;
        sf::Vector3f m_position;
    };
    float m_intensity;

    Light(Type type, float intensity, sf::Vector3f dir_or_pos = sf::Vector3f(0, 0, 0));
};

class LightManager
{
    unsigned int m_count = 0;
    std::vector<Light*> m_lights = {};

    public:

    LightManager() = default;
    ~LightManager();

    unsigned int size() const { return m_count; }
    void add(Light* obj);

    Light& operator[](int index) { return *m_lights[index]; }
    const Light& operator[](int index) const { return *m_lights[index]; }
};