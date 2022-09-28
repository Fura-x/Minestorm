
#include <stdio.h>

#include "../include/SATcollision.h"

bool    pointPolygonCollide(float2 pt, convex_polygon polygon)
{
    segment*    segs = new segment[polygon.nPts];
    getPolygonSegments(polygon, segs);

    line*       lines = new line[polygon.nPts];
    getLinesFromSegments(segs, lines, polygon.nPts);

    bool is_collision = true;
    for (int i = 0; i < polygon.nPts && is_collision; ++i)
    {
        float2 n = lines[i].dir.getNormal();
        is_collision = isRangeCollision(polygonProjection(polygon, n), pointProjection(pt, n));
    }
    
    delete[] segs;
    delete[] lines;

    return is_collision;
}


bool    circlePolygonCollide(circle circle, convex_polygon polygon)
{
    segment*    segs = new segment[polygon.nPts];
    getPolygonSegments(polygon, segs);

    line*        lines = new line[polygon.nPts];
    getLinesFromSegments(segs, lines, polygon.nPts);

    bool is_collision =true;
    for (int i = 0; i < polygon.nPts && is_collision; ++i)
    {
        float2 n = lines[i].dir.getNormal();
        is_collision = isRangeCollision(polygonProjection(polygon, n), circleProjection(circle, n));
    }
    float2 near_pt = getNearPoint(polygon.pts, circle.center, polygon.nPts);
    float2 dir = getUnit(circle.center - near_pt);

    delete[] segs;
    delete[] lines;

    if (is_collision)
        return  isRangeCollision(polygonProjection(polygon, dir), circleProjection(circle, dir));
    else
        return false;
}

bool    circlePolygonCollide(circle cir, concave_polygon concave)
{
    bool isCollision = false;

    for (int i = 0; i < concave.polygonCount && !isCollision; ++i)
        isCollision = circlePolygonCollide(cir, concave.polygons[i]);

    return isCollision;
}


bool    boxPolygonCollide(box b, convex_polygon polygon)
{
    float2  points[4];
    getBoxPoints(b, points);

    convex_polygon boxPolygon = { points, 4 };

    return PolygonsCollide(boxPolygon, polygon);
}

//CONVEX-CONVEX
bool    PolygonsCollide(convex_polygon polygon1, convex_polygon polygon2)
{
    //get segments of polygon1
    segment*    segs1 = new segment[polygon1.nPts];
    getPolygonSegments(polygon1, segs1);
    line*       lines1 = new line[polygon1.nPts];
    getLinesFromSegments(segs1, lines1, polygon1.nPts);

    //get segments of polygon2
    segment*    segs2 = new segment[polygon2.nPts];
    getPolygonSegments(polygon2, segs2);
    line*       lines2 = new line[polygon2.nPts];
    getLinesFromSegments(segs2, lines2, polygon2.nPts);

    bool is_collision = true;
    for (int i = 0; i < polygon1.nPts && is_collision; ++i)
    {
        float2 n = lines1[i].dir.getNormal();
        is_collision = isRangeCollision(polygonProjection(polygon1, n), polygonProjection(polygon2, n));
    }
    for (int i = 0; i < polygon2.nPts && is_collision; ++i)
    {
        float2 n = lines2[i].dir.getNormal();
        is_collision = isRangeCollision(polygonProjection(polygon1, n), polygonProjection(polygon2, n));
    }

    delete[] segs1;
    delete[] segs2;
    delete[] lines1;
    delete[] lines2;

    return is_collision;
}

//CONVEX-CONCAVE
bool    PolygonsCollide(convex_polygon convex, concave_polygon concave)
{
    bool isCollision = false;

    for (int i = 0; i < concave.polygonCount && !isCollision; ++i)
        isCollision = PolygonsCollide(convex, concave.polygons[i]);

    return isCollision;
}


//CONCAVE-CONCAVE
bool    PolygonsCollide(concave_polygon concave1, concave_polygon concave2)
{
    bool isCollision = false;

    for (int i = 0; i < concave1.polygonCount && !isCollision; ++i)
    {
        for (int j = 0; j < concave2.polygonCount && !isCollision; ++j)
            isCollision = PolygonsCollide(concave1.polygons[i], concave2.polygons[j]);
    }

    return isCollision;
}
