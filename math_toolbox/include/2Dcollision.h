#pragma once

#include "primitive.h"

//point collision
bool	pointsCollide(float2 pt1, float2 pt2);
bool	pointLineCollide(float2 pt, line li);
bool	pointSegmentCollide(float2 pt, segment seg);
bool	pointCircleCollide(float2 pt, circle cir);
bool	pointBoxCollide(float2 pt, box rect);

//line collision
bool	linesCollide(line li1, line li2);
bool	lineSegmentCollide(line li, segment seg);
bool	lineCircleCollide(line li, circle cir);
bool	lineBoxCollide(line li, box rect);

//segment collision
bool	segmentsCollide(segment seg1, segment seg2);
bool	segmentCircleCollide(segment seg, circle cir);
bool	segmentBoxCollide(segment seg, box rect);

//circle collision
bool	circlesCollide(circle cir1, circle cir2);
bool	circleBoxCollide(circle cir, box rect);

//box collision
bool	boxsCollide(box rect1, box rect2);


