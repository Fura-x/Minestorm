
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "primitive.h"

#define EPSILON pow(2, -24)
#define M_PI 3.14159265359f

//useful functions
float	    clamp(float x, float min, float max)
{
    x = x < min ? min : x;
    return x > max ? max : x;
}

float	    getMin(float a, float b)
{
    return a < b ? a : b;
}

float	    getMax(float a, float b)
{
    return a > b ? a : b;
}

float2     getNearPoint(float2* pts, float2 pt, const int count)
{
    float*   distances = new float[count];
    for (int i = 0; i < count; ++i)
        distances[i] = getDistance(pts[i], pt);
    
    float   min;
    int     index = 0;
    float   prevMin = distances[index];
    for (int i = 0; i < count-1; ++i)
    {
        min = getMin(distances[i], distances[i+1]);
        if (prevMin > min)
        {
            prevMin = min;
            index = i;
        }
    }
    delete[] distances;
    return pts[index];
}



//angle convert
float       radianToDegrees(float radian)
{
    return  radian * 180.f / M_PI;
}

float       degreesToRadian(float degrees)
{
    return degrees * M_PI / 180.f;
}

//vector definition
float2     getNullVector()
{
    return {0, 0};
}

bool       cmpVector2(float2 v1, float2 v2)
{
    return ((fabsf(v1.x - v2.x) <= EPSILON) && (fabsf(v1.y - v2.y) <= EPSILON));
}

float       getLength(float2 v)
{
   return sqrt(v.x*v.x + v.y*v.y);
}

float       getSquareLength(float2 v)
{
    return v.x*v.x + v.y*v.y;
}

float2       getVector2(float2 pt1, float2 pt2)
{
    return pt2 - pt1;
}

float       getDistance(float2 pt1, float2 pt2)
{
    return getLength(getVector2(pt1, pt2));
}

float2     getUnit(float2 v)
{
    float   length = getLength(v);

    if (length == 0)
        return getNullVector();
    
    return {v.x / length, v.y / length};
}


//specific vector operation

float       getAngleDegrees(float2 v1, float2 v2)
{
    return radianToDegrees(getAngle(v1,v2));
}


float2     pointRotation(float2 pt, float2 center, float angle)
{
    float2 v = getVector2(center, pt).getRotated(angle);

    return v + center;
}


//referential change
float2     posLocalToGlobal(float2 pos, referential2 local_ref)
{
    return pos + local_ref.origin;
    /*
    float2 globalVect = local_ref.origin + (local_ref.unitX * pos.x);
    globalVect = globalVect + (local_ref.unitY * pos.y);

    return globalVect;*/
}

float2     posGlobalToLocal(float2 pos, referential2 local_ref)
{
    float2 origin_pos = getVector2(local_ref.origin, pos);
    float   local_x = dotProduct(origin_pos, local_ref.unitX);
    float   local_y = dotProduct(origin_pos, local_ref.unitY);

    return {local_x, local_y};
}

float2     vectLocalToGlobal(float2 v, referential2 local_ref)
{
    float2 globalVect = (local_ref.unitX * v.x) + (local_ref.unitY * v.y);

    return  globalVect;
}

float2     vectGlobalToLocal(float2 v, referential2 local_ref)
{
    float   local_x = dotProduct(v, local_ref.unitX);
    float   local_y = dotProduct(v, local_ref.unitY);

    return {local_x, local_y};
}


//segment operation
line        getLineFromSegment(segment seg)
{
    float2 dir = getVector2(seg.pt1, seg.pt2);
    return {seg.pt1, dir};
}

void        getLinesFromSegments(segment* segs, line* lines, int count)
{
    for (int i = 0; i < count; ++i)
        lines[i] = getLineFromSegment(segs[i]);
}



//box operations
float2     getBoxPoint(box box, int which)
{
    if (which == RIGHT_UP)
        return {box.center.x + box.width_ha, box.center.y + box.height_ha};
    else if (which == RIGHT_DOWN)
        return {box.center.x + box.width_ha, box.center.y - box.height_ha};
    else if (which == LEFT_DOWN)
        return {box.center.x - box.width_ha, box.center.y - box.height_ha};
    else //if LEFT_UP or random which
        return {box.center.x - box.width_ha, box.center.y + box.height_ha};
}

void    getBoxPoints(box box, float2* pts)
{
    for (int i = 0; i < 4; ++i)
	    pts[i] = getBoxPoint(box, i);
}

convex_polygon  boxPolygon(box box)
{
    float2  points[4];
    getBoxPoints(box, points);

    return { points, 4 };
}

//range operation
range       rangeMerge(range range1, range range2)
{
    return {getMin(range1.min, range2.min), getMax(range1.max, range2.max)};
}

range       rangeMergeByValue(range range1, float value)
{
    return {getMin(range1.min, value), getMax(range1.max, value)};
}

bool        isRangeCollision(range range1, range range2)
{
    return !((range1.min > range2.max) || (range2.min > range1.max));
}

range       pointProjection(float2 pt, float2 dir)
{
    float projection = dotProduct(pt, dir);
    return {projection, projection};
}

range       segmentProjection(segment seg, float2 dir)
{
    float proj1 = dotProduct(seg.pt1, dir);
    float proj2 = dotProduct(seg.pt2, dir);

    return rangeMerge({proj1, proj2}, {proj2, proj1});
}

range       boxProjection(box box, float2 dir)
{
    float proj[4];

    for (int i = 0; i < 4; ++i)
        proj[i] = dotProduct(getBoxPoint(box, i), dir);

    return rangeMerge({proj[0], proj[1]}, {proj[2], proj[3]});
}

range       polygonProjection(const convex_polygon polygon, float2 dir)
{
    range*   proj = new range[polygon.nPts];
    for (int i = 0; i < polygon.nPts; ++i)
    {
        proj[i] = pointProjection(polygon.pts[i], dir);
    }

    range trueProj = proj[0];
    for (int i = 1; i < polygon.nPts; ++i)
        trueProj = rangeMerge(trueProj, proj[i]);

    delete[] proj;

    return trueProj;
}


range       circleProjection(circle cir, float2 dir)
{
    float proj = dotProduct(cir.center, dir);

    return {proj - cir.radius, proj + cir.radius};
    // float2 proj1 = vector_subs(cir.center, vector_scale(dir, cir.radius));
    // float2 proj2 = vector_add(cir.center, vector_scale(dir, cir.radius));

    // return rangeMerge(pointProjection(proj1, dir), pointProjection(proj2, dir));
}

//polygon operation
void        getPolygonSegments(convex_polygon polygon, segment* segs)
{
    for (int i = 0; i < polygon.nPts-1; ++i)
        segs[i] = {polygon.pts[i], polygon.pts[i+1]};
    segs[polygon.nPts-1] = {polygon.pts[polygon.nPts-1], polygon.pts[0]};
}

box         getPolygonAABB(convex_polygon convex)
{
    float minX = 1000.f, maxX = -1000.f, minY = 1000.f, maxY = -1000.f;
    for (int i = 0; i < convex.nPts; ++i)
    {
        auto& p = convex.pts[i];
        minX = getMin(minX, p.x);
        maxX = getMax(maxX, p.x);
        minY = getMin(minY, p.y);
        maxY = getMax(maxY, p.y);
    }
    float2 center = { (minX + maxX) / 2, (minY + maxY) / 2 };
    return { center , center.x - minX, center.y - minY };
}


//print funcitions

void	printFloat2(float2 v)
{
    printf("Coord : x = %.2f, y = %.2f\n", v.x, v.y);
}

void	printRange(range r)
{
    printf("Min = %.2f, Max = %.2f\n", r.min, r.max);
}
