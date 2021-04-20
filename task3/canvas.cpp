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


Canvas::~Canvas()
{
    delete [] m_framebuffer;
}