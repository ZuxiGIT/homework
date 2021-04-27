#pragma once

template <typename T>
struct Vector2
{
    T x, y;

    Vector2() : x(0), y(0) {}
    Vector2(T X, T Y) : x(X), y(Y) {}
    template<typename U>
    explicit Vector2(const Vector2<U>& that) : x(static_cast<T>(that.x)), y(static_cast<T>(that.y)) {}

    Vector2<T> operator-() { return Vector2<T> (-x, -y); }
    Vector2<T>& operator+=(const Vector2<T>& that) { x += that.x; y += that.y; return *this; }
    Vector2<T>& operator-=(const Vector2<T>& that) { x -= that.x; y -= that.y; return *this; } 
    Vector2<T>& operator*=(T that) { x *= that; y *= that; return *this; }
    Vector2<T>& operator/=(T that) { x /= that; y /= that; return *this; }
    bool operator==(const Vector2<T>& that) { return (x == that.x) && (y = that.y); }
    bool operator!=(const Vector2<T>& that) { return !(*this == that); }

};

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    Vector2<T> temp = lhs;
    return temp += rhs;
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    Vector2<T> temp = lhs;
    return temp -= rhs;
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T>& lhs, T num)
{
    Vector2<T> temp = lhs;
    return temp *= num;
}

template <typename T>
inline Vector2<T> operator/(const Vector2<T>& lhs, T num)
{
    Vector2<T> temp = lhs;
    return temp /= num;
}

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2c = Vector2<char>;

template <typename T>
struct Vector3
{
    T x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    template<typename U>
    explicit Vector3(const Vector3<U>& that) :  x(static_cast<T>(that.x)), y(static_cast<T>(that.y)), \
                                                z(static_cast<T>(that.z)) {}
    
    Vector3<T> operator-() { return Vector3<T> (-x, -y, -z); }
    Vector3<T>& operator+=(const Vector3<T>& that) { x += that.x; y += that.y; z += that.z; return *this; }
    Vector3<T>& operator-=(const Vector3<T>& that) { x -= that.x; y -= that.y; z -= that.z; return *this; } 
    Vector3<T>& operator*=(T that) { x *= that; y *= that; z *= that; return *this; }
    Vector3<T>& operator/=(T that) { x /= that; y /= that; z /= that; return *this; }
    bool operator==(const Vector3<T>& that) { return (x == that.x) && (y = that.y) && (z == that.z); }
    bool operator!=(const Vector3<T>& that) { return !(*this == that); }
};

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
    Vector3<T> temp = lhs;
    return temp += rhs;
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
    Vector3<T> temp = lhs;
    return temp -= rhs;
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T>& lhs, T num)
{
    Vector3<T> temp = lhs;
    return temp *= num;
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T>& lhs, T num)
{
    Vector3<T> temp = lhs;
    return temp /= num;
}