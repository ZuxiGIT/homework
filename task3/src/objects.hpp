#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "noncopyable.hpp"
#include "color.hpp"
#include <vector>
#include <string>
#include <ctime>


extern const float _INFINITY;
extern const float _EPS;

struct Material
{
    int specular = 1;
    float reflective = 0;
};

struct  Drawable : public NonCopyable
{
    enum class Type
    {
        PLANE,
        SPHERE
    };

    Type m_type;
    sf::Vector3f m_position = {};
    Material m_properties = {};
    sf::Color m_color = {};

    Drawable(sf::Vector3f position, sf::Color color, Material properties, Type type)
    :
    m_type(type),
    m_position(position),
    m_properties(properties),
    m_color(color)
    {}

    Drawable(const Drawable&) = delete;
    Drawable& operator=(const Drawable&) = delete;

    Type  getType() { return m_type; };
    virtual sf::Vector3f ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const = 0;
    virtual sf::Vector3f getNormal(const sf::Vector3f& point) const = 0;
    sf::Color getColor() const  { return m_color; }
    sf::Vector3f getPosition() const { return m_position; }

    virtual ~Drawable() { fprintf(stderr, "----> %s was called on %d line\n", __PRETTY_FUNCTION__, __LINE__); }

};

std::string operator+(const Drawable::Type& type);


class Sphere : public Drawable
{
    float m_radius = 0;

public:
    explicit Sphere(sf::Vector3f position, float radius, Material properties, sf::Color color = sf::Color::Green);
    Sphere() = default;

    virtual sf::Vector3f ray_intersect (const sf::Vector3f& origin, const sf::Vector3f& direction) const override;
    virtual sf::Vector3f getNormal(const sf::Vector3f& point) const override { return point - m_position;}
    
    virtual ~Sphere() { fprintf(stderr, "----> %s was called on %d line\n", __PRETTY_FUNCTION__, __LINE__); }
    

    static Sphere* generate()
    {
        std::srand(std::time(nullptr));

        sf::Vector3f pos {std::rand() % 10, std::rand() % 10, std::rand() % 10 };
        sf::Color col {std::rand() % 255, std::rand() % 255, std::rand() % 255 };
        Material prop = {500, 0.9f};
        
        return new Sphere{pos, 1, prop, col};
    }
};


class Plane : public Drawable
{
    sf::Vector3f m_plane_normal = {};
public:

    explicit Plane(sf::Vector3f plane_point, sf::Vector3f plane_normal, Material properties, sf::Color color = sf::Color::Magenta);

    virtual sf::Vector3f ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const override;
    virtual sf::Vector3f getNormal(const sf::Vector3f&) const override { return m_plane_normal; }
    
    virtual ~Plane() { fprintf(stderr, "----> %s was called on %d line\n", __PRETTY_FUNCTION__, __LINE__); }

    static Plane* generate()
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        sf::Vector3f pos {0, 0, 50};
        sf::Vector3f normal {static_cast<float>(std::rand() % 10), 0, static_cast<float>(std::rand() % 10)};
        Material prop {1, 0};
        sf::Color col {static_cast<unsigned char>(std::rand() % 255), static_cast<unsigned char>(std::rand() % 255), static_cast<unsigned char>(std::rand() % 255) };
        fprintf(stderr, "color (%d, %d, %d)\n", col.r, col.g, col.b);
        return new Plane {pos, normal, prop, col};
    }
};

class ObjectManager
{
    std::vector<Drawable*> m_objects = {};

    ObjectManager() = default;
    ~ObjectManager();

public:
    static ObjectManager& createManager()
    { static ObjectManager instance; return instance; }

    ObjectManager(ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;

    void remove(size_t index);
    size_t size() const { return m_objects.size(); }
    
    Drawable& operator[](size_t index) const { return *m_objects[index]; }
    //const Drawable& operator[] (size_t index) const  { return *m_objects[index]; }

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
    std::vector<Light*> m_lights = {};

    public:

    LightManager() = default;
    ~LightManager();

    size_t size() const { return m_lights.size(); }
    void add(Light* obj);

    Light& operator[](int index) { return *m_lights[index]; }
    const Light& operator[](int index) const { return *m_lights[index]; }
};