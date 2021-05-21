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

void Camera::rotate(float x, float y)
{
    Vector2f zx = rot(x);
    fprintf(stderr, "rot(x) (%f, %f)\n", zx.x, zx.y);
    m_direction.z = m_direction.z * zx.x - m_direction.x * zx.y;
    m_direction.x = m_direction.z * zx.y + m_direction.x * zx.x;

    m_camera_right.z = m_camera_right.z * zx.x - m_camera_right.x * zx.y;
    m_camera_right.x = m_camera_right.z * zx.y + m_camera_right.x * zx.x;


    Vector2f xy = rot(y);
    fprintf(stderr, "rot(y) (%f, %f)\n", xy.x, xy.y);
    m_direction.y = m_direction.y * xy.x - m_direction.z * xy.y;
    m_direction.z = m_direction.y * xy.y + m_direction.z * xy.x;

    m_camera_up.y = m_camera_up.y * xy.x - m_camera_up.z * xy.y;
    m_camera_up.z = m_camera_up.y * xy.y + m_camera_up.z * xy.x;

}

Vector2f Camera::rot(float a)
{
    float s = static_cast<float>(sin(a));
    float c = static_cast<float>(cos(a));
    return Vector2f(c, s);
}

sf::Vector3f Camera::CanvasToViewPort(unsigned int x, unsigned int y) const
{
    sf::Vector3f res = {};
    res += m_position;
    res += normalize(m_direction);
    res +=  (static_cast<float>(x) / static_cast<float>(m_canvas_size.x) - 0.5f) * normalize(m_camera_right);
    res += -(static_cast<float>(y) / static_cast<float>(m_canvas_size.y) - 0.5f) * normalize(m_camera_up);
    return res;
}
