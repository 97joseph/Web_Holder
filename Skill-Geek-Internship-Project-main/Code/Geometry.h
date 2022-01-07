#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <iostream>
#include <memory>
#include <map>
#include <vector>

class Point;


// Abstract class
class Shape {

public:
	Shape(int d);

	bool setDepth(int d);                            // Set object depth
	int  getDepth() const;                           // Get object depth

	virtual int  dim() const = 0;                    // Get object dimension
	virtual void translate(float x, float y) = 0;    // Translate the object by x and y
	virtual void rotate() = 0;                       // Rotate the object 90 degrees around its centre
	virtual void scale(float f) = 0;                 // Scale the object by a factor f relative to its centre
	virtual bool contains(const Point& p) const = 0; // Check if the object contains p
    
	static constexpr double PI = 3.1415926;          // the constant pi

private:
    int objectDepth;                                 // Depth of the object
};


class Point : public Shape {

public:
	Point(float x, float y, int d = 0);

    // Getters
	float getX() const;
	float getY() const;

    // Overriding methods
    int  dim() const override final;
    void translate(float x, float y) override final;
    void rotate() override final;
    void scale(float f) override final;
    bool contains(const Point& p) const override final;

private:
    // Coordinates of the point
    float x, y;
};


class LineSegment : public Shape {

public:
	LineSegment(const Point& p, const Point& q);

	// Get end-point coordinates
	float getXmin() const;
	float getXmax() const;
	float getYmin() const;
	float getYmax() const;
	
    // Get length of the line segment
    float length() const;
    
    // Overriding methods
    int  dim() const override final;
    void translate(float x, float y) override final;
    void rotate() override final;
    void scale(float f) override final;
    bool contains(const Point& p) const override final;

private:
    // End-points coordinates
    float x1, y1;
    float x2, y2;
};


// Abstract class
class TwoDShape : public Shape {

public:
	TwoDShape(int d);

    // All 2DShapes are 2 dimensioned, So, no further inheritance required.
	int dim() const override final;
    
    // Both rectangle and circle has an area attribute, hence area() declared as virtual to be overriden.
    virtual float area() const = 0;
};


class Rectangle : public TwoDShape {

public:
	Rectangle(const Point& p, const Point& q);

	// Get corner coordinates
	float getXmin() const;
	float getYmin() const;
	float getXmax() const;
	float getYmax() const;

    // Overriding methods
    void  translate(float x, float y) override final;
    void  rotate() override final;
    void  scale(float f) override final;
    bool  contains(const Point& p) const override final;
    float area() const override final;

private:
    // Coordinates of the four vertices of a rectangle.
    float x1, y1, x2, y2, x3, y3, x4, y4;

    // Array of pointers pointing to the coordiantes, for easier iteration to perform operations
    float *xCoor[4] = { &x1, &x2, &x3, &x4 };
    float *yCoor[4] = { &y1, &y2, &y3, &y4 };
};


class Circle : public TwoDShape {
public:
	Circle(const Point& c, float r);

	// Get center point of the circle
	float getX() const;
	float getY() const;
    
	// Get radius of the circle
    float getR() const;

    // Overriding methods
    void  translate(float x, float y) override final;
    void  rotate() override final;
    void  scale(float f) override final;
    bool  contains(const Point& p) const override final;
	float area() const override final;

private:
    // Center coordinate and radius of the circle
    float x, y, radius;
};


class Scene {

public:
	Scene();

	// Add the pointer to the collection of pointers stored
	void addObject(std::shared_ptr<Shape> ptr);

	// Set the drawing depth to d
	void setDrawDepth(int d);

	// Constants specifying the size of the drawing area
	static constexpr int WIDTH = 60;
	static constexpr int HEIGHT = 20;

private:
    // Once turned on, objects with depths no greater than the drawDepth wil be drawn
    bool hasCustomDepth;

    // Specifies the master drawing depth
    int  drawDepth;

    // Used map to group objects associated with same depth
    // Mapped int (depth) to list of pointers to Shape object (vector<pointers>) for constant time retrieval O(1)
    std::map< int, std::vector<std::shared_ptr<Shape>> > objectList;
    

// Redirect the coordinate plane to output stream object "out"
friend std::ostream& operator<<(std::ostream& out, const Scene& s);

// Checks if a cell in the plane should be shaded or marked empty
friend bool shadePoint(const Scene& s, const Point& p);
};

#endif /* GEOMETRY_H_ */