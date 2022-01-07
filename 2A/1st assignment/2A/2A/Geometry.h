#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <iostream>
#include <cmath>
#include <memory>
#include "stack.cpp"

class Point; // forward declaration

class Shape {
public:
	// Default constructor, just to make this release version compilable.
	// If your implementation is correct this should be removed
	Shape();

	// Constructor specifying the depth of the object.
	// If d is negative, throw a std::invalid_argument exception.
	Shape(int d);

	// Set depth of object to d. If d is negative, return false and
	// do not update depth. Otherwise return true
	bool setDepth(int d);

	// Return the depth of object
	int getDepth() const;

	// Return the dimension of the object (0, 1 or 2)
	//virtual int dim()=0;
	virtual int dim()const=0;
	// Translate the object horizontally by x and vertically by y
	virtual void translate(float x, float y)=0;

	// Rotate the object 90 degrees around its centre
	virtual void rotate()=0;

	// Scale the object by a factor f relative to its centre.
	// If f is zero or negative, throw a std::invalid-argument exception.
	virtual void scale(float f)=0;

	// Return true if the object contains p and false otherwise.
	// Depths are ignored for purpose of comparison
	virtual bool contains(const Point& p)=0;// const;

	// the constant pi
	static constexpr double PI = 3.1415926;
	int depth;
protected:
private:
	// add any protected/private member variables you need
};

class Point : public Shape {

public:
	float x;
	float y;
	// Constructor. Depth defaults to 0
	Point();
	Point(float x, float y);
	Point(float x, float y, int d);
	// Return basic information (see assignment page)
	float getX() const;
	float getY() const;
	//int dim() const;
	void translate(float x, float y);
//	void translate(float x, float y)
//	{
//		this->x += x;
//		this->y += y;
////		cout << "x : " << this->x << "y: " << this->y;
//	}

	void rotate();
	void scale(float f);
	bool contains(const Point&p);
	bool setDepth(int);
	int getDepth();
	int dim()const;
private:
	// add any member variables you need
};

class LineSegment : public Shape {

public:
	Point start;
	Point end;
	// Constructor.
	// If the two points have different depths, or have the same x- and
	// y-coordinate, or if the line is not axis-aligned, throw a
	// std::invalid_argument exception
	LineSegment(const Point& p, const Point& q);

	// Return basic information (see assignment page)
	float getXmin() const;
	float getXmax() const;
	float getYmin() const;
	float getYmax() const;

	// Return the length of the line segment
	float length() const;
//	void translate(float x, float y);
	void translate(float x, float y){
		start.translate(x, y);
		end.translate(x, y);
	}
	void rotate();
	bool contains(const Point&);
	void scale(float f);
	int dim()const;

private:
	// add any member variables you need
};

class TwoDShape : public Shape {

public:
	// Default constructor.
	// Similar comment to Student default constructor applies
	TwoDShape();

	// Constructor specifying the depth d
	TwoDShape(int d);
	virtual void translate(float x, float y)=0;
	virtual void rotate()=0;
	virtual void scale(float f)=0;
//	int dim()const;

	// Return the area of the object
	virtual float area() const=0;

protected:
private:
	// add any protected/private member variables you need
};

class Rectangle : public TwoDShape {

public:
	// Constructor.
	Rectangle();
	// If the two points have different depths, or have the same x-
	// and/or y-coordinate, throw a std::invalid_argument exception
	Rectangle(const Point& p, const Point& q);

	// Return basic information (see assignment page)
	float getXmin() const;
	float getYmin() const;
	float getXmax() const;
	float getYmax() const;
	int getDepth() const;

//	void translate(float x, float y);
	void translate(float x, float y){
		p1.translate(x, y);
		p2.translate(x, y);
	}
	void rotate();
	void scale(float f);
	bool contains(const Point&);
	float area()const;
	int dim()const;
	
private:
	Point p1;
	Point p2;
	// add any member variables you need
};

class Circle : public TwoDShape {

public:
	Circle();
	// Constructor.
	// If r is zero or negative, throw a std::invalid-argument exception.
	Circle(const Point& c, float r);

	// Return basic information (see assignment page)
	float getX() const;
	float getY() const;
	float getR() const;
//	void translate(float x, float y);
	void translate(float x, float y){
		center.translate(x, y);
	}
	void rotate();
	void scale(float f);
	bool contains(const Point&p);
	float area()const;
	int dim()const;	

private:
	// add any member variables you need
	Point center;
	float radius;
	const float PI=3.1415;
};


class Scene {
//	Stack <shared_ptr<Shape>> dstr;
public:
	// Constructor
	Scene();

	// Add the pointer to the collection of pointers stored
	void addObject(std::shared_ptr<Shape> ptr);

	// Set the drawing depth to d
	void setDrawDepth(int d);

	// Constants specifying the size of the drawing area
	static constexpr int WIDTH = 60;
	static constexpr int HEIGHT = 20;

	//bool check_if_Point_in_any_object_of_Stack(const Point & p);
private:
	// add any member variables you need
	Stack <std::shared_ptr<Shape>> *dstr;
	int drawDepth;
	// Draw objects as specified in the assignment page
friend std::ostream& operator<<(std::ostream& out, const Scene& s);
};

//template <typename T>
//class Node
//{
//private:
//	T data;
//	Node<T>* next;
//
//public:
//
//	Node();
////	{
////		data = 0;
////		next = NULL;
////	}
//
//	Node(T value);
////	{
////		data = value;
////		next = NULL;
////	}
//
//	Node <T>* getNext();
////	{
////		return next;
////	}
//
//	void setnext(Node<T>* ptr);
////	{
////		next = ptr;
////	}
//
//	T getData();
////	{
////		return data;
////	}
//
//	void setData(T value);
////	{
////		data = value;
////	}
//
//};
//
//
//template <typename T>
//class Stack
//{
//	Node<T>* top;
//
//public:
//	Stack();
////	{
////		top = NULL;
////	}
//
//	Stack(Node<T>* ptr);
////	{
////		top = ptr;
////	}
//
//	Node <T>* getTop();
////	{
////		return top->getData();
////	}
//
//	void setTop(Node<T>* ptr);
////	{
////		top = ptr;
////	}
//
//	bool isEmpty();
////	{
////		if (top == NULL)
////		{
////			return true;
////		}
////		else
////			return false;
////	}
//
//	void push(T value);
////	{
////		Node<T>* temp = new Node <T>(value);
////
////		if (top == NULL)
////		{
////			top = temp;
////		}
////		else
////		{
////			temp->setnext(top);
////			top = temp;
////		}
////
////	}
//
//	int length();
////	{
////		Node<T>* current = top;
////		int count = 0;
////		while (current != NULL) {
////			current = current->getNext();
////			count++;
////		}
////		return count * 1;
////	}
//	bool pop();
////	{
////
////		if (top == NULL)
////		{
////			return false;
////		}
////		else
////		{
////			Node<T>* temp = top;
////			top = top->getNext();
////			delete temp;
////
////			return true;
////		}
////	}
//	T returner(int i);
//// {
////		int count = 0;
////		Node<T>* current = top;
////		while (count < i) {
////			current = current->getNext();
////			count++;
////		}
////		return current->getData();
////	}
//	T peek();
////	{
////		if (isEmpty())
////			return NULL;
////		else
////			return top->getData();
////	}
//};


#endif /* GEOMETRY_H_ */	

