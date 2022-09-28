#pragma once

#include <iostream>
#include "raylib.h"

class float2
{
private:

public:
    float           x = 0.f;
    float           y = 0.f;
    static float2   i;
    static float2   j;


    float2();
    float2(float _x, float _y);
    float2(const float2& f);
    ~float2() = default;

    operator float*();

    float2& operator=(const float2& f);
    bool operator==(const float2& f);

    float2  operator-() const;
    void    operator-();
    void    operator+=(const float2& f);
    float2  operator+(const float2& f);
    void    operator-=(const float2& f);
    float2  operator-(const float2& f);

    void    operator*=(const float2& f);
    float2  operator*(const float f);
    float2  operator*(const float2 f);
    float2  operator/(const float f);

    float   operator[](const int index) const;

    float   length();
    float   squareLength();
    float2  normalize();
    float2  getRotated(float rotation);
    float2  getNormal();
    operator Vector2();
};

std::ostream& operator<<(std::ostream&, float2 f);
float   dotProduct(float2 v1, float2 v2);
float   getAngle(float2 v1, float2 v2);
