// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "camera.hpp"
#include "functions.inl"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

extern const float _EPS;

Camera::Camera(const sf::Vector3f& position, const sf::Vector3f& direction)
:
m_position(position),
m_direction(normalize(direction))
{
    sf::Vector3f up {0.f, 1.f, 0.f};
    m_camera_left = normalize(cross(up, m_direction));
    m_camera_up = normalize(cross(m_direction, m_camera_left));
    
    fprintf(stderr, "m_direction (%f, %f, %f)\n", m_direction.x, m_direction.y, m_direction.z);
    fprintf(stderr, "camera_up (%f, %f, %f)\n", m_camera_up.x, m_camera_up.y, m_camera_up.z);
    fprintf(stderr, "m_camera_left (%f, %f, %f)\n", m_camera_left.x, m_camera_left.y, m_camera_left.z);

}

void Camera::setCanvasSize(unsigned int width, unsigned int height)
{
    m_canvas_size = sf::Vector2i(width, height);
}

void Camera::rotate(double x_angle, double y_angle)
{
    // const static double _PI = 3.14159265358979323846;
    // fprintf(stderr, "rotate angle is %lf deg\n", x_angle / _PI * 180);

        if(fabs(y_angle) > _EPS)
    {
        Vector2f yz = rot(y_angle);
        //fprintf(stderr, "rot(y) (%f, %f)\n", xy.x, xy.y);
        
        // yz.x --> cos(x_angle)
        // yz.y --> sin(x_angle)
        
        m_camera_up.y = m_camera_up.y * yz.x - m_camera_up.z * yz.y;
        m_camera_up.z = m_camera_up.y * yz.y + m_camera_up.z * yz.x;

        m_direction.y = m_direction.y * yz.x - m_direction.z * yz.y;
        m_direction.z = m_direction.y * yz.y + m_direction.z * yz.x;

        m_direction = normalize(m_direction);
        m_camera_up = normalize(m_camera_up);
    }


    if(fabs(x_angle) > _EPS)
    {
        Vector2f zx = rot(x_angle);
        // fprintf(stderr, "rot(x) (%f, %f)\n", zx.x, zx.y);

        // zx.x --> cos(x_angle)
        // zx.y --> sin(x_angle)

        m_direction.z = m_direction.z * zx.x - m_direction.x * zx.y;
        m_direction.x = m_direction.z * zx.y + m_direction.x * zx.x;

        m_camera_left.z = m_camera_left.z * zx.x - m_camera_left.x * zx.y;
        m_camera_left.x = m_camera_left.z * zx.y + m_camera_left.x * zx.x;

        m_direction = normalize(m_direction);
        m_camera_left = normalize(m_camera_left);
    }
}

sf::Vector3f Camera::CanvasToViewPort(unsigned int x, unsigned int y) const
{
    sf::Vector3f res = {};
    res += m_position;
    res += normalize(m_direction);
    res +=  (static_cast<float>(x) / static_cast<float>(m_canvas_size.x) - 0.5f) * (m_camera_left);
    res += -(static_cast<float>(y) / static_cast<float>(m_canvas_size.y) - 0.5f) * (m_camera_up);
    return res;
}
