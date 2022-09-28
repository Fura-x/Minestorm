
#include <stdio.h>
#include <math.h>

#include "../include/2Dcollision.h"

#define EPSILON powf(2, -24)

//point collision
bool	pointsCollide(float2 pt1, float2 pt2)
{
    //don't use getDistance to avoid squareroot calcul
    return getSquareLength(getVector2(pt1, pt2)) < EPSILON;	
}

bool	pointLineCollide(float2 pt, line li)
{
    return fabsf(dotProduct(getVector2(pt, li.pt), li.dir.getNormal()))< EPSILON;
}

bool	pointSegmentCollide(float2 pt, segment seg)
{
    float2 dir = getVector2(seg.pt1, seg.pt2); 
    line    li = {seg.pt1, dir};

    if (!pointLineCollide(pt, li))
	return false;
    
    return fabsf(dotProduct(getVector2(seg.pt1, pt), dir)) < getLength(dir);
}

bool	pointCircleCollide(float2 pt, circle cir)
{
    return getDistance(cir.center, pt) <= cir.radius;
}

bool	pointBoxCollide(float2 pt, box rect)
{
    return (fabsf(pt.x - rect.center.x) <= rect.width_ha) && (fabsf(pt.y - rect.center.y) <= rect.height_ha);
}


//line collision
bool	linesCollide(line li1, line li2)
{
    if (dotProduct(li1.dir.getNormal(), li2.dir) != 0)
	    return true;

    return pointLineCollide(li1.pt, li2);
}

bool	lineSegmentCollide(line li, segment seg)
{
    float2 v1 = getVector2(li.pt, seg.pt1);
    float2 v2 = getVector2(li.pt, seg.pt2);
    float2 n = li.dir.getNormal();

    return (dotProduct(v1, n) * dotProduct(v2, n) <= 0);
    // if (dotProduct(v1, n) * dotProduct(v2, n) > 0)
	//     return false;
    // else
	//     return line_line(li, (line){seg.pt1, getVector2(seg.pt1, seg.pt2)}); 
}

bool	lineCircleCollide(line li, circle cir)
{
    float2 v = getVector2 (cir.center, li.pt);
    return fabsf(dotProduct(v, li.dir.getNormal())) <= cir.radius;
}

bool	lineBoxCollide(line li, box rect)
{
    float2 pts[4];
    getBoxPoints(rect, pts);
    
    segment segs[4];
    for (int i = 0; i < 3; ++i)
	    segs[i] = {pts[i], pts[i+1]};
    segs[3] = {pts[3], pts[0]};

    bool   corner_col[4];
    for (int i = 0; i < 4; ++i)
	    corner_col[i] = pointLineCollide(pts[i], li);

    return lineSegmentCollide(li, segs[0]) || lineSegmentCollide(li, segs[2])
            || lineSegmentCollide(li, segs[1]) || lineSegmentCollide(li, segs[3])
            ||corner_col[0] || corner_col[1] || corner_col[2] || corner_col[3];
}

//segment collision
bool	segmentsCollide(segment seg1, segment seg2)
{
    line li1 = {seg1.pt1, getVector2(seg1.pt1, seg1.pt2)};
    line li2 = {seg2.pt1, getVector2(seg2.pt1, seg2.pt2)};

    if (!linesCollide(li1, li2))
	    return false;

    range range1 = rangeMerge({seg1.pt1.x, seg1.pt2.x}, {seg1.pt2.x, seg1.pt1.x});
    range range2 = rangeMerge({seg2.pt1.x, seg2.pt2.x}, {seg2.pt2.x, seg2.pt1.x});

    return isRangeCollision(range1, range2);
}

bool	segmentCircleCollide(segment seg, circle cir)
{
    line    li = {seg.pt1, getVector2(seg.pt1, seg.pt2)};
    
    if (!lineCircleCollide(li, cir))
        return false;
    else if (pointCircleCollide(seg.pt1, cir) || pointCircleCollide(seg.pt2, cir))
        return true;

    return ((seg.pt1.x - cir.center.x) * (seg.pt2.x - cir.center.x) <= EPSILON) 
	|| ((seg.pt1.y - cir.center.y) * (seg.pt2.y - cir.center.y) <= EPSILON);
}

bool	segmentBoxCollide(segment seg, box rect)
{
    line li = {seg.pt1, getVector2(seg.pt1, seg.pt2)};

    if (!lineBoxCollide(li, rect))
	return false;
    else if (pointBoxCollide(seg.pt1, rect) || pointBoxCollide(seg.pt2, rect))
	return true;

    float2 pts[4];
    getBoxPoints(rect, pts);

    segment segs[4];
    for (int i = 0; i < 3; ++i)
	segs[i] = {pts[i], pts[i+1]};
    segs[3] = {pts[3], pts[0]};

    return segmentsCollide(seg, segs[0]) || segmentsCollide(seg, segs[1])
	|| segmentsCollide(seg, segs[2]) || segmentsCollide(seg, segs[3]);
}

//circle collision
bool	circlesCollide(circle cir1, circle cir2)
{
    return cir1.radius + cir2.radius >= getDistance(cir1.center, cir2.center);
}

bool	circleBoxCollide(circle cir, box rect)
{
    if (pointBoxCollide(cir.center, rect))
        return true;
    
    float2 near_pt;
    near_pt.x = clamp(cir.center.x, rect.center.x - rect.width_ha, rect.center.x + rect.width_ha);
    near_pt.y = clamp(cir.center.y, rect.center.y - rect.height_ha, rect.center.y + rect.height_ha);

    return pointCircleCollide(near_pt, cir);
}

//box collision
bool	boxsCollide(box rect1, box rect2)
{
    range   range1X = {rect1.center.x - rect1.width_ha, rect1.center.x + rect1.width_ha};
    range   range1Y = {rect1.center.y - rect1.height_ha, rect1.center.y + rect1.height_ha};
    range   range2X = {rect2.center.x - rect2.width_ha, rect2.center.x + rect2.width_ha};
    range   range2Y = {rect2.center.y - rect2.height_ha, rect2.center.y + rect2.height_ha};

    return isRangeCollision(range1X, range2X) && isRangeCollision(range1Y, range2Y);
}

