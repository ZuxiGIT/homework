#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Config.hpp>
#include "noncopyable.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "objects.hpp"

class Canvas : public NonCopyable
{
    static const Color BACKGROUND;
    unsigned int m_width = {};
    unsigned int m_height = {};
    sf::Uint8* m_framebuffer = {};
    sf::RenderWindow* m_window = nullptr;
    ObjectManager* m_objects = nullptr;
    LightManager* m_lights = nullptr;
    Camera* m_camera = nullptr;
    sf::Texture m_texture = {};
    sf::Sprite m_sprite = {};

    Canvas(const Canvas&) = delete;
    Canvas& operator= (const Canvas&) = delete;
    
    void renderer();
public:
    explicit Canvas(unsigned int width, unsigned int height);
    void setPixel(unsigned int x, unsigned int y, Color pixel);
    void setObjects(ObjectManager& objects) { m_objects = &objects; }
    void setLights(LightManager& lights) { m_lights = &lights; }
    void setCamera(Camera& cam) { m_camera = &cam; m_camera->setCanvasSize(m_width, m_height); }
    void draw(sf::RenderTarget& target);
    ~Canvas();
};