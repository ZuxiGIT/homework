#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>



class Drawable
{
public:
    virtual bool ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const = 0;
    virtual sf::Vector3f getColor() const = 0;
    virtual ~Drawable() {}

};



class Sphere : public Drawable
{
    sf::Vector3f m_center = {};
    float m_radius = 0;
    sf::Color m_color = {};

    public:

    explicit Sphere(sf::Vector3f position, float radius, sf::Color color = sf::Color::Green);
    Sphere() = default;

    Sphere(const Sphere&) = delete;
    Sphere& operator= (const Sphere&) = delete;

    virtual bool ray_intersect (const sf::Vector3f& origin, const sf::Vector3f& direction) const override;
    virtual sf::Vector3f getColor() const override { return sf::Vector3f(m_color.r / 255, m_color.g / 255, m_color.b / 255); }

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