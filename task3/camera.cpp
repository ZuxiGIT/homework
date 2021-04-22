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
    // fprintf(stderr, "0) m_canvas_size (%d, %d)\n", m_canvas_size.x, m_canvas_size.y);
    sf::Vector3f res = {};
    // fprintf(stderr, "1)res = (%f, %f, %f)\n", res.x, res.y, res.z);
    res += m_position;
    // fprintf(stderr, "2)res = (%f, %f, %f)\n", res.x, res.y, res.z);
    res += m_direction;
    // fprintf(stderr, "3)res = (%f, %f, %f)\n", res.x, res.y, res.z);
    res += ((float) x / (float)(m_canvas_size.x) - 0.5f) * m_camera_right;
    // fprintf(stderr, "4)res = (%f, %f, %f)\n", res.x, res.y, res.z);
    res += -((float) y / (float)(m_canvas_size.y) - 0.5f) * m_camera_up;
    // fprintf(stderr, "5)res = (%f, %f, %f)\n", res.x, res.y, res.z);
    // fprintf(stderr, "5.1)m_camera_up = (%f, %f, %f)\n", m_camera_up.x, m_camera_up.y, m_camera_up.z);
    return res;
}
