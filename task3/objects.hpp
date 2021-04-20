#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

const float _INFINITY = static_cast<float>(0xDEADBEEF);
const float _EPS = static_cast<float>(1e-10);

struct Color
{
    float r = 0;
    float g = 0;
    float b = 0;

    Color() = default;
    
    Color( float R, float G, float B){ 
                                        r = R > 1 ? 1 : R;
                                        g = G > 1 ? 1 : G;
                                        b = B > 1 ? 1 : B;
                                     }
    
    Color( const sf::Vector3f& that){  
                                        r = that.x  > 1 ? 1 : that.x;
                                        g = that.y  > 1 ? 1 : that.y;
                                        b = that.z  > 1 ? 1 : that.z;
                                    }
    Color( const sf::Color& that) { r = that.r / 255; g = that.g / 255; b = that.b / 255; }
    
    Color(const Color& obj) { r = obj.r; g = obj.g; b = obj.b; }
    
    Color& operator=(const Color& obj) { r = obj.r; g = obj.g; b = obj.b; return *this; }
    
    ~Color() = default;
    
    operator sf::Vector3f() const { return sf::Vector3f( r, g, b); } 
};

template<typename T>
inline Color operator*(T num, const Color& rhs)
{
    return Color (rhs.r * num, rhs.g * num, rhs.b * num); 
}
template<typename T>
inline Color operator*(const Color& rhs, T num)
{
    return num * rhs;
}
template<typename T>
inline Color operator/(const Color& rhs, T num)
{
    return Color( rhs.r / num, rhs.g / num, rhs.b / num);
}


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

    virtual sf::Vector3f ray_intersect(const sf::Vector3f& origin, const sf::Vector3f& direction) const = 0;
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

    Sphere(const Sphere&) = delete;
    Sphere& operator= (const Sphere&) = delete;

    virtual sf::Vector3f ray_intersect (const sf::Vector3f& origin, const sf::Vector3f& direction) const override;
    virtual ~Sphere() override {}
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