#pragma once
#include "noncopyable.hpp"
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

class Camera : public NonCopyable
{
    sf::Vector3f m_position = {};
    sf::Vector3f m_direction = {};
    sf::Vector3f m_camera_up = {};
    sf::Vector3f m_camera_right = {};
    sf::Vector2i m_canvas_size = {};
    
public:
    sf::Vector3f getPosition() { return m_position; }
    void setCanvasSize(unsigned int width, unsigned int height);
    explicit Camera(const sf::Vector3f& position, const sf::Vector3f& direction);
    sf::Vector3f CanvasToViewPort(unsigned int x, unsigned int y) const;
};
