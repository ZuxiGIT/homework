#include <SFML/Graphics.hpp>
#include "canvas.hpp"
#include "ray.hpp"
#include "raytrace.hpp"

Canvas::Canvas(unsigned int width, unsigned int height, float xLTop, float yLTop)
:
m_width(width),
m_height(height),
m_framebuffer(new sf::Uint8 [width * height * 4]),
m_position(xLTop, yLTop)
{
    m_texture.create(m_width, m_height);
    m_sprite.setPosition(m_position.x, m_position.y);
}

void Canvas::setPixel(unsigned int x, unsigned int y, Color pixel)
{
    m_framebuffer[(y * m_width + x) * 4]     = (unsigned char) (255 * pixel.r);
    m_framebuffer[(y * m_width + x) * 4 + 1] = (unsigned char) (255 * pixel.g);
    m_framebuffer[(y * m_width + x) * 4 + 2] = (unsigned char) (255 * pixel.b);
    m_framebuffer[(y * m_width + x) * 4 + 3] = (unsigned char) (255);
}

void Canvas::renderer()
{
   
    for (size_t line = 0; line < m_height; line++)
        for(size_t column = 0; column < m_width; column++)
        {
            sf::Vector3f dir  = m_camera->CanvasToViewPort(column, line);
            sf::Vector3f origin  = m_camera->getPosition();
            //fprintf(stderr, "direction is %lf %lf %lf\n", dir.x, dir.y, dir.z);
            Ray ray(origin, dir);
            Color res = Raytrace::ray_cast(ray, 1, _INFINITY, *m_objects, *m_lights);
            
            setPixel(column, line, res);
        }
    
    for (size_t line = 0; line < m_height; line++)
        setPixel(m_width / 2, line, sf::Vector3f(0, 0,0));
    for (size_t column = 0; column < m_width; column++)
        setPixel(column, m_height / 2, sf::Vector3f(0, 0,0));
    
    m_texture.update(m_framebuffer);
    m_sprite.setTexture(m_texture, true);    
}

bool Canvas::isInCanvas(const Vector2f& mouse_pos)
{
    float local_x = mouse_pos.x - m_position.x;
    float local_y = mouse_pos.y - m_position.y;
    
    if( local_x < static_cast<float>(m_height) && local_x > 0 && 
        local_y < static_cast<float>(m_width)  && local_y > 0)
        return true;
    
    return false;
}

void Canvas::draw(sf::RenderTarget& target)
{
    renderer();
    target.draw(m_sprite);
}

Canvas::~Canvas()
{
    delete [] m_framebuffer;
}