#pragma once
#include "SFML/Config.hpp"
#include "color.hpp"

class Canvas
{
    unsigned int m_width = {};
    unsigned int m_height = {};
    sf::Uint8* m_framebuffer = {};

public:
    explicit Canvas(unsigned int width, unsigned int height);
    void setPixel(unsigned int x, unsigned int y, Color pixel);

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    ~Canvas();
};