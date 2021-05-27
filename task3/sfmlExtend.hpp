#pragma once
#include <SFML/Graphics/Shape.hpp>
#include "mymath.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace sf{

    class EllipseShape final : public Shape
    {
        Vector2f m_radius = {};
        size_t m_quality = {};
    public:
        explicit EllipseShape(const Vector2f& radius = Vector2f(0.f, 0.f), size_t pointCount = 30)
        : 
        m_radius(radius),
        m_quality(pointCount)
        {
            update();
        }

        virtual ~EllipseShape() override {}
        
        void setRadius(const Vector2f radius)
        {
            m_radius = radius;
            update();
        }

        const Vector2f& getRadius() const
        {
            return m_radius;
        }

        virtual size_t getPointCount() const override
        {
            return m_quality;
        }

        virtual Vector2f getPoint(size_t index) const override
        {
            double angle = index * 2 * M_PI / getPointCount() - M_PI / 2;

            float x = static_cast<float>(cos(angle) * static_cast<float>(m_radius.x));
            float y = static_cast<float>(sin(angle) * static_cast<float>(m_radius.y));

            return Vector2f(m_radius.x + x, m_radius.y + y);
        }
    };
}