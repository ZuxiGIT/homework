#include "camera.hpp"
#include "functions.inl"
#include <stdio.h>

Camera::Camera(const sf::Vector3f& position, const sf::Vector3f& direction)
:
m_position(position),
m_direction(normalize(direction))
{
    sf::Vector3f up {0.f, 1.f, 0.f};
    m_camera_right = normalize(cross(up, m_direction));
    m_camera_up = normalize(cross(m_direction, m_camera_right));
}

void Camera::setCanvasSize(unsigned int width, unsigned int height)
{
    m_canvas_size = sf::Vector2i(width, height);
}

sf::Vector3f Camera::CanvasToViewPort(unsigned int x, unsigned int y) const
{
    sf::Vector3f res = {};
    res += m_position;
    res += m_direction;
    res += ((float) x / (float)(m_canvas_size.x) - 0.5f) * m_camera_right;
    res += -((float) y / (float)(m_canvas_size.y) - 0.5f) * m_camera_up;
    return res;
}
