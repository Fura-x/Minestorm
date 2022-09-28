#pragma once

#include <stdbool.h>
#include <vector>

#include "float2.h"

enum e_box_points
{
    RIGHT_UP,
    RIGHT_DOWN,
    LEFT_DOWN,
    LEFT_UP
};

struct referential2
{
    float2 origin = { 0.f, 0.f };
    float2 unitX = { 1.f, 0.f };
    float2 unitY = { 0.f, 1.f };
    float  angle = 0.f;
};

struct range
{
    float   min = 0.f;
    float   max = 0.f;
} ;

struct line
{
    float2 pt = { 0.f, 0.f };
    float2 dir = { 1.f, 1.f };
};

struct segment
{
    float2 pt1 = { -1, -1 };
    float2 pt2 = { 1 , 1 };
};

struct circle
{
    float2 center = { 0.f, 0.f };
    float  radius = { 1.f };
};

struct box
{
    float2  center = { 0.f, 0.f };
    float   width_ha = 1.f;
    float   height_ha = 1.f;
};

struct oriented_box
{
    box     rect;
    float   angle = 0.f;
};

struct convex_polygon
{
    float2* pts;
    int     nPts = 0;
};

struct concave_polygon
{
    convex_polygon* polygons = nullptr;
    int             polygonCount = 0;
};


//useful function
float	    clamp(float x, float min, float max);
float	    getMin(float a, float b);
float	    getMax(float a, float b);
float2      getNearPoint(float2*, float2, const int count);

//angle convert
float       radianToDegrees(float radian);
float       degreesToRadian(float degrees);

//vector definition
float2     getNullVector();
float2     getUnit(float2 v);
bool       cmpVector2(float2 v1, float2 v2);
float      getLength(float2 v);
float      getSquareLength(float2 v);
float2     getVector2(float2 pt1, float2 pt2);
float      getDistance(float2 pt1, float2 pt2);

//specific vector operation
float      getAngleDegrees(float2 v1, float2 v2);
float2     pointRotation(float2 pt, float2 center, float angle);

//referential change
float2     posLocalToGlobal(float2 pos, referential2 local_ref);
float2     posGlobalToLocal(float2 pos, referential2 local_ref);
float2     vectLocalToGlobal(float2 v, referential2 local_ref);
float2     vectGlobalToLocal(float2 v, referential2 local_ref);

//segment operation
line        getLineFromSegment(segment);
void        getLinesFromSegments(segment*, line*, int count);

//box operation
float2          getBoxPoint(box box, int which);
void            getBoxPoints(box rect, float2* pts);
convex_polygon  boxPolygon(box box);

//range operation
range       rangeMerge(range range1, range range2);
range       rangeMergeByValue(range range1, float value);
bool        isRangeCollision(range range1, range range2);
range       pointProjection(float2 pt, float2 dir);
range       segmentProjection(segment seg, float2 dir);
range       boxProjection(box box, float2 dir);
range       polygonProjection(const convex_polygon, float2 dir);
range       circleProjection(circle cir, float2 dir);


//polygon operation
void        getPolygonSegments(convex_polygon, segment*);
box         getPolygonAABB(convex_polygon);

//print function
void	    printFloat2(float2 v);
void	    printRange(range r);
