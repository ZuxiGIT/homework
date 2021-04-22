#include "canvas.hpp"

Canvas::Canvas(unsigned int width, unsigned int height)
:
m_width(width),
m_height(height),
m_framebuffer(new sf::Uint8 [width * height * 4])
{}

void Canvas::setPixel(unsigned int x, unsigned int y, Color pixel)
{
    m_framebuffer[(y * m_width + x) * 4]     = (unsigned char) (255 * pixel.r);
    m_framebuffer[(y * m_width + x) * 4 + 1] = (unsigned char) (255 * pixel.g);
    m_framebuffer[(y * m_width + x) * 4 + 2] = (unsigned char) (255 * pixel.b);
    m_framebuffer[(y * m_width + x) * 4 + 3] = (unsigned char) (255);
}

void Canvas::renderer()
{
   #if 0
    for (size_t line = 0; line < m_height; line++)
        for(size_t column = 0; column < m_width; column++)
        {
            sf::Vector3f dir  = m_camera->CanvasToViewPort(column, line);
            //fprintf(stderr, "direction is %lf %lf %lf\n", dir.x, dir.y, dir.z);
            
            Color res = ray::ray_cast(sf::Vector3f(0,0,0), dir, *m_objects, *m_lights);
            
            setPixel(column, line, res);
        }
    #endif
    for (size_t line = 0; line < m_height; line++)
        setPixel(m_width / 2, line, sf::Vector3f(0, 0,0));
    for (size_t column = 0; column < m_width; column++)
        setPixel(column, m_height / 2, sf::Vector3f(0, 0,0));
}


Canvas::~Canvas()
{
    delete [] m_framebuffer;
}