
#include <cmath>

#include "float2.h"

#define M_PI 3.14159265f

float2   float2::i = {1.f, 0.f};
float2   float2::j = {0.f, 1.f};


float2::float2()
:x(0.f), y(0.f)
{}


float2::float2(float _x, float _y)
//:x(_x), y(_y), z(_z)
{
    x = _x;
    y = _y;
}

float2::float2(const float2& f)
:x(f.x), y(f.y)
{}

float2::operator float*()
{
    return &x;  
}


float2& float2::operator=(const float2& f)
{
    x = f.x; 
    y = f.y;
    return *this;
}

bool float2::operator==(const float2& f)
{
    return ((f.x - x) <= (float)1E-54 && (f.y - y) <= (float)1E-54);
}


float2  float2::operator-() const
{
    float2 f = {-x, -y};
    return f;
}

void float2::operator-()
{
    x = -x;
    y = -y;
}

void    float2::operator+=(const float2& f)
{
    x += f.x;
    y += f.y;
}

float2  float2::operator+(const float2& f)
{
    return { x + f.x, y + f.y };
}


void    float2::operator-=(const float2& f)
{
    *this += -f;
}

float2  float2::operator-(const float2& f)
{
    return { x - f.x, y - f.y };
}

void    float2::operator*=(const float2& f)
{
    x *= f.x;
    y *= f.y;
}

float2    float2::operator*(const float f)
{
    return {x * f, y * f};
}

float2  float2::operator*(const float2 f)
{
    return { x * f.x, y * f.y };
}

float2    float2::operator/(const float f)
{
    if (f != 0)
        return { x / f, y / f };
    else
        return { 0.f, 0.f };
}


float   float2::operator[](const int index) const
{
    return *(&x + index);
}

float   dotProduct(float2 v1, float2 v2)
{
    return { v1.x * v2.x + v1.y * v2.y };
}

float   getAngle(float2 v1, float2 v2)
{
    float   normsProduct = (v1.length() * v2.length());
    if (normsProduct == 0.f)
        return 0.f;

    float   dotProd = dotProduct(v1, v2);
    return fabsf(dotProd) < normsProduct ? acosf(dotProd / normsProduct) : 0.f;
}


float   float2::length()
{
    return sqrtf(dotProduct(*this, *this));
}

float   float2::squareLength()
{
    return x * x + y * y;
}


float2  float2::normalize()
{
    float l = length();
    return { x / l, y / l };
}

float2  float2::getRotated(float rotation)
{
    float2 rotated = {x, y};

    rotation = rotation * 3.14f / 180.f; //maths::degreesToRadian(rotation);

    rotated.x = x * cosf(rotation) - y  * sinf(rotation);
    rotated.y = x * sinf(rotation) + y * cosf(rotation);

    return rotated;
}

float2  float2::getNormal()
{
    *this = this->normalize();
    return { -y, x };
}


float2::operator Vector2()
{
    return { x, y };
}


std::ostream& operator<<(std::ostream& o, float2 f)
{
    return o << "x : " << f.x << ", y : " << f.y;
}


