#include <math.h>
#include <map>

#include "Geometry.h"
// #include "geometry.cpp"

/*
	If there is any linker error at all, please uncomment the include directive for geometry.cpp.
	I know it's a bad practice to do so, but a quick workaround
*/


// ============ Utility functions ================

inline void swap(float& a, float& b) { // Inline function to swap two float values
    float t { a };
    a = b;
    b = t;
}


// ============ Shape class =================

Shape::Shape(int d) {
    if (d < 0)
        throw std::invalid_argument("Negative depth!");
    
    objectDepth = d;
}

bool Shape::setDepth(int d) {
    if (d < 0)
        return false;
    
    objectDepth = d;
    return true;
}

int Shape::getDepth() const {
    return objectDepth;
}


// =============== Point class ================

Point::Point(float x, float y, int d) : Shape(d) {  // Call Shape parameterized ctor to set depth
    // Assignment of x and y coordinates
    this->x = x;
    this->y = y;
}

float Point::getX() const {
    return x;
}

float Point::getY() const {
    return y;
}

int Point::dim() const {
    // A point is 0 dimensioned
    return 0;
}

void Point::translate(float x, float y) {
    // Increment/Decrement point's coordinate by x and y
    this->x += x;
    this->y += y;
}

void Point::rotate() {} // Rotation of a point has no effect

void Point::scale(float f) {
    if (f <= 0)
        throw std::invalid_argument("Negative scale factor"); 
    // Scaling of a point has no effect
}

bool Point::contains(const Point& p) const {
    // Contains only if both coordinates are equal
    return (p.getX() == x && p.getY() == y);
}


// =========== LineSegment class ==============

LineSegment::LineSegment(const Point& p, const Point& q) : Shape(0) { // Call Shape() ctor to set depth temporarily as 0
    // Exceptions
    if (p.getDepth() != q.getDepth())
        throw std::invalid_argument("Points depth mismatch");
    else if (p.getX() != q.getX() && p.getY() != q.getY())
        throw std::invalid_argument("Line is not axis-aligned");
    else if (p.getX() == q.getX() && p.getY() == q.getY())
        throw std::invalid_argument("Points conincide");

    // Set depth as the points are valid
    setDepth(p.getDepth());

    if (p.getX() == q.getX()) { // Line parallel to Y-axis
        x1 = x2 = p.getX();
        
        y1 = std::min(p.getY(), q.getY());
        y2 = std::max(p.getY(), q.getY());
    }
    else {                      // Line parallel to X-axis
        y1 = y2 = p.getY();
        
        x1 = std::min(p.getX(), q.getX());
        x2 = std::max(p.getX(), q.getX());
    }
}

float LineSegment::getXmin() const {
    return x1;
}

float LineSegment::getXmax() const {
    return x2;
}

float LineSegment::getYmin() const {
    return y1;
}

float LineSegment::getYmax() const {
    return y2;
}

float LineSegment::length() const {
    // Distance formula
    return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

int LineSegment::dim() const {
    // A line segment is 1 dimensional
    return 1;
}

void LineSegment::translate(float x, float y) {
    // Increment/Decrement both the point's coordinates by x and y
    x1 += x; y1 += y;
    x2 += x; y2 += y;
}

void LineSegment::rotate() {
    // Store coordinates of the mid point of the line
    float cX, cY;
    // Store temporary coordinates
    float tX, tY;
    
    // Swapping end-points as rotation will change orientation
    if (x1 == x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    
    // Calculate mid-point of the line segment
    cX = (x1 + x2) / 2;
    cY = (y1 + y2) / 2;

    // Move the line to origin center
    x1 -= cX; y1 -= cY;
    x2 -= cX; y2 -= cY;

    // Rotate x, y
    tX = x1 ; tY = y1;
    x1 =- tY; y1 = tX; 

    tX = x2 ; tY = y2;
    x2 =- tY; y2 = tX; 

    // Revert back to original position
    x1 += cX; y1 += cY;
    x2 += cX; y2 += cY;
}

void LineSegment::scale(float f) {
    float cX, cY;
    
    if (f <= 0)
        throw std::invalid_argument("Negative scale factor");
    
    // Mid-point
    cX = (x1 + x2) / 2;
    cY = (y1 + y2) / 2;

    // Move the line to origin center
    x1 -= cX; y1 -= cY;
    x2 -= cX; y2 -= cY;
    
    // Multiply coorindates with given scale factor, with origin as center of dilation
    x1 *= f; y1 *= f;
    x2 *= f; y2 *= f;
    
    // Shift back the figure to initial position
    x1 += cX; y1 += cY;
    x2 += cX; y2 += cY;
}

bool LineSegment::contains(const Point& p) const {
    if (x1 == x2) // Line parallel to Y-axis
        return (p.getX() == x1 && p.getY() >= std::min(y1, y2) && p.getY() <= std::max(y1, y2));
    else          // Line parallel to X-axis
        return (p.getY() == y1 && p.getX() >= std::min(x1, x2) && p.getX() <= std::max(x1, x2));
}


// ============ TwoDShape class ================

TwoDShape::TwoDShape(int d) : Shape(d) {} // No additional functionality or operations to be performed

int TwoDShape::dim() const {
    // All 2D Shapes are 2 dimensional
    return 2;
}

// ============== Rectangle class ================

Rectangle::Rectangle(const Point& p, const Point& q) : TwoDShape(0) {
    // Exceptions
    if (p.getDepth() != q.getDepth())
        throw std::invalid_argument("Depth mismatch");
    else if (p.getX() == q.getX() || p.getY() == q.getY())
        throw std::invalid_argument("Lines coincide");
    
    // Set depth as points are valid
    setDepth(p.getDepth());
    
    // Initialize coordinates of the rectangle
    // 4 cases in which the given diagonally opposite points (P and Q) would be situated
    
    if (p.getX() < q.getX() && p.getY() < q.getY()) {       // Px < Qx, Py < Qy
        x1 = p.getX(); y1 = p.getY();
        x3 = q.getX(); y3 = q.getY();
        
        // Opposite points
        x2 = q.getX(); y2 = p.getY();
        x4 = p.getX(); y4 = q.getY();
    }
    else if (p.getX() < q.getX() && p.getY() > q.getY()) {  // Px < Qx, Py > Qy
        x2 = q.getX(); y2 = q.getY();
        x4 = p.getX(); y4 = p.getY();
        
        // Opposite points
        x1 = p.getX(); y1 = q.getY();
        x3 = q.getX(); y3 = p.getY();
    }
    else if (p.getX() > q.getX() && p.getY() > q.getY()) {  // Px > Qx, Py > Qy
        x1 = q.getX(); y1 = q.getY();
        x3 = p.getX(); y3 = p.getY();
        
        // Opposite points
        x2 = p.getX(); y2 = q.getY();
        x4 = q.getX(); y4 = p.getY();
    }
    else {                                                  // Px > Qx, Py < Qy
        x2 = p.getX(); y2 = p.getY();
        x4 = q.getX(); y4 = q.getY();
        
        // Opposite points
        x1 = q.getX(); y1 = p.getY();
        x3 = p.getX(); y3 = q.getY();
    }
}

float Rectangle::getXmin() const {
    return x1;
}

float Rectangle::getYmin() const {
    return y1;
}

float Rectangle::getXmax() const {
    return x3;
}

float Rectangle::getYmax() const {
    return y3;
}

float Rectangle::area() const {
    float length, breath;
    
    // Distance formula to find length and breath
    length = sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
    breath = sqrtf(powf(x3 - x2, 2) + powf(y3 - y2, 2));
    
    // Area (Rectangle) = Length * Breath
    return length * breath;
}

void Rectangle::translate(float x, float y) {
    // Looping from (x1, y1) to (x4, y4) and appying translation accorindly
    for (size_t i {0}; i < 4; i++) {
        *xCoor[i] += x;
        *yCoor[i] += y;
    }
}

void Rectangle::rotate() {
    float cX, cY, xTemp, yTemp;
    
    // Find min-point of the rectangle
    cX = (x1 + x2) / 2;
    cY = (y1 + y4) / 2;
    
    // Shift to origin
    for (size_t i {0}; i < 4; i++) {
        *xCoor[i] -= cX;
        *yCoor[i] -= cY;
    }

    /* Using rotation matrix to obtain rotated coordinates
    
     * Rotation matrix:
     * [ cos ϕ -sin ϕ ]
     * [ sin ϕ  cos ϕ ]
     * 
     * Degree to radian = ϕ = 90 Degrees × Pi/180 => 1.5708
     * 
     * [ 0  1 ]
     * [ -1 0 ]
    */
    
    for (size_t i {0}; i < 4; i++) {
        xTemp = *xCoor[i];
        yTemp = *yCoor[i];
        
        // Multiplying (x, y) with rotation matrix and shifting back to original position
        *xCoor[i] = (0 * xTemp) + (-1 * yTemp) + cX;
        *yCoor[i] = (1 * xTemp) + (0  * yTemp) + cY;
    }
    
    // Swap points as orientation changed
    swap(x1, x4); swap(y1, y4);
    swap(x2, x3); swap(y2, y3);
    
    // If lines parallel to x-axis are misaligned, swap to fix them
    if (y1 != y2) {
        swap(x2, x4);
        swap(y2, y4);
    }
}


void Rectangle::scale(float f) {
    if (f <= 0)
        throw std::invalid_argument("Negative scale factor");

    float cX, cY;
    
    // Mid-point of the rectangle
    cX = (x1 + x2) / 2;
    cY = (y1 + y4) / 2;
    
    // Shift to origin
    for (size_t i {0}; i < 4; i++) {
        *xCoor[i] -= cX;
        *yCoor[i] -= cY;
    }
    
    // Multiply scale factor to all vertices
    for (size_t i {0}; i < 4; i++) {
        *xCoor[i] *= f;
        *yCoor[i] *= f;
    }
    
    // Shift back the object to it's initial position
    for (size_t i {0}; i < 4; i++) {
        *xCoor[i] += cX;
        *yCoor[i] += cY;
    }
}

bool Rectangle::contains(const Point& p) const {
    // Check if the given point coordinate lie within the rectangle's extreme points A and C
    return (p.getX() >= x1 && p.getX() <= x3 && p.getY() >= y1 && p.getY() <= y3);
}


// ================== Circle class ===================

Circle::Circle(const Point& c, float r) : TwoDShape(0) {
    if (r <= 0)
        throw std::invalid_argument("Invalid argument!");
    
    // Set depth as arguments are valid
    setDepth(c.getDepth());
    
    // Initialize coordinates of the center of the circle
    x = c.getX();
    y = c.getY();
    
    // Initialize radius to r
    radius = r;
}

float Circle::getX() const {
    return x;
}

float Circle::getY() const {
    return y;
}

float Circle::getR() const {
    return radius;
}


float Circle::area() const {
    // Area (Circle) = πr2
    return PI * (radius * radius);
}

void Circle::translate(float x, float y) {
    // Apply translation to x and y coordinate
    this->x += x;
    this->y += y;
}

void Circle::rotate() { } // Rotation of circle has no effect

void Circle::scale(float f) {
    if (f <= 0)
        throw std::invalid_argument("Negative scale factor");
    
    // Multiply scale factor with radius to scale the circle
    radius *= f;
}

bool Circle::contains(const Point& p) const {
    float dist;
    
    // Calculate distance from center of the circle to point p
    dist = sqrtf(powf(p.getX() - x, 2) + powf(p.getY() - y, 2));
    
    // P is inside circle, if dist(p, c) is less than or equal to the radius of the circle
    return (dist <= radius);
}

// ================= Scene class ===================

Scene::Scene() {
    // Boolean to indicate, if the scene class has a custom drawing depth
    hasCustomDepth = false;
    
    // drawDepth holds the value, in case if depth is specified by user
    drawDepth = -1;
}

void Scene::addObject(std::shared_ptr<Shape> ptr) {
    // Depth of the object to be added to the scene class
    int objectDepth = ptr->getDepth();
    
    // Check if any object exists with same depth
    if (objectList.find(objectDepth) != objectList.end()) {
        // Append new object to the list of existing objects with same depth
        objectList[objectDepth].push_back(ptr);
    }
    else {
        // Populate the vector with a pointer to a shape object with "objectDepth" depth
        objectList[objectDepth] = std::vector<std::shared_ptr<Shape>>(1, ptr);
    }
}

void Scene::setDrawDepth(int depth) {
	if (depth < 0)
        throw std::invalid_argument("Negative depth!");
    
    // Trigger the switch to indicate that the user has specified drawing depth
    hasCustomDepth = true;
    
    // Assign the user-specified value to drawDepth
    drawDepth = depth;
}

bool shadePoint(const Scene& s, const Point& p) {
    // Iterate through the pairs of integers and vectors in the map objectList
    // P is the pair(int, vector<shared_ptr<Shape>>)
    // listItem is the vector of Shape pointers, which is the second item in the pair
    
    for (auto P: s.objectList) {
        for (auto listItem: P.second) {
        
            // If draw depth is specified by the user, and the item's depth exceeds the draw depth, return false
            if (s.hasCustomDepth && s.drawDepth < listItem->getDepth())
                    return false;
        
            // If draw depth is not specified or the item's depth is lesser than or equal to the draw depth, return true
            if (listItem->contains(p))
                return true;
        }
    }
    
    // If there is no objects in the vector that contains the point 'p' meaning that it is a empty cell, return false
    return false;
}

std::ostream& operator<<(std::ostream& out, const Scene& s) {
    // Looping points(x, y) from (0, 0) to (59, 19)
    for (int a {0}; a < s.HEIGHT; a++) {
        for (int b {0}; b < s.WIDTH; b++) {
            
            /* Point object to store current point, where
                x = (b)
                y = (height - a - 1) simple workaround to iterate from reverse in an forward loop
            */
            Point currentPosition (b, s.HEIGHT - a - 1);
            
            // If shadePoint returns true, fill the cell with '*'
            // Else fill the cell with a whitespace character
            
            if (shadePoint(s, currentPosition))
                out << '*';
            else
                out << ' ';
        }
        
        // End current line, equivalent to increment the Y coordinate
        out << std::endl;
    }
    
	return out;
}