#pragma once

#include "primitive.h"

bool    pointPolygonCollide(float2, convex_polygon);
bool    circlePolygonCollide(circle, convex_polygon);
bool    circlePolygonCollide(circle, concave_polygon);
bool    boxPolygonCollide(box, convex_polygon);
bool    PolygonsCollide(convex_polygon, convex_polygon);
bool    PolygonsCollide(convex_polygon, concave_polygon);
bool    PolygonsCollide(concave_polygon, concave_polygon);
