#pragma once
#include "noncopyable.hpp"
#include "mymath.hpp"
#include <string>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

class Camera : public NonCopyable
{
public:
    sf::Vector3f m_position = {};
    sf::Vector3f m_direction = {};
    sf::Vector3f m_camera_up = {};
    sf::Vector3f m_camera_left = {};
    sf::Vector2i m_canvas_size = {};

    
    sf::Vector3f getPosition() { return m_position; }
    void setCanvasSize(unsigned int width, unsigned int height);
    explicit Camera(const sf::Vector3f& position, const sf::Vector3f& direction);
    void rotate(double x_angle, double y_angle);
    sf::Vector3f CanvasToViewPort(unsigned int x, unsigned int y) const;
};
