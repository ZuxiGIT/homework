#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Config.hpp>
#include "noncopyable.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "objects.hpp"
#include "mymath.hpp"


class Canvas : public NonCopyable
{
    static const Color BACKGROUND;
    
    unsigned int m_width    = {};
    unsigned int m_height   = {};
    
    sf::Uint8* m_framebuffer    = {};
    Vector2f m_position         = {};
    
    sf::RenderWindow* m_window  = nullptr;
    ObjectManager* m_objects    = nullptr;
    LightManager* m_lights      = nullptr;
    Camera* m_camera            = nullptr;
    
    sf::Texture m_texture   = {};
    sf::Sprite  m_sprite    = {};

    Canvas(const Canvas&)               = delete;
    Canvas& operator= (const Canvas&)   = delete;
    
    void renderer();
public:
    explicit Canvas(unsigned int width, unsigned int height, float xLTop = 0, float yLTop = 0);

    void setPixel(unsigned int x, unsigned int y, RGB pixel);
    
    void setObjects(ObjectManager& objects) { m_objects = &objects; }
    void setLights(LightManager& lights)    { m_lights = &lights; }
    void setCamera(Camera& cam)             { m_camera = &cam; m_camera->setCanvasSize(m_width, m_height); }
    
    bool isInCanvas(const Vector2f& mouse_pos);
    void draw(sf::RenderTarget& target);
    
    ~Canvas();
};