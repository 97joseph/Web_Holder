#include <iostream>
#include <cmath>
#include <memory>
using namespace std;

template <typename T>
class Node
{
private:
	T data;
	Node<T>* next;

public:

	Node()
	{
		data = 0;
		next = NULL;
	}

	Node(T value)
	{
		data = value;
		next = NULL;
	}

	Node <T>* getNext()
	{
		return next;
	}

	void setnext(Node<T>* ptr)
	{
		next = ptr;
	}

	T getData()
	{
		return data;
	}

	void setData(T value)
	{
		data = value;
	}

};


////////////////////////////////////////////////

template <typename T>
class Stack
{
	Node<T>* top;

public:
	Stack()
	{
		top = NULL;
	}

	Stack(Node<T>* ptr)
	{
		top = ptr;
	}

	Node <T>* getTop()
	{
		return top->getData();
	}

	void setTop(Node<T>* ptr)
	{
		top = ptr;
	}

	bool isEmpty()
	{
		if (top == NULL)
		{
			return true;
		}
		else
			return false;
	}

	void push(T value)
	{
		Node<T>* temp = new Node <T>(value);

		if (top == NULL)
		{
			top = temp;
		}
		else
		{
			temp->setnext(top);
			top = temp;
		}

	}

	int length() {
		Node<T>* current = top;
		int count = 0;
		while (current != NULL) {
			current = current->getNext();
			count++;
		}
		return count * 1;
	}
	bool pop()
	{

		if (top == NULL)
		{
			return false;
		}
		else
		{
			Node<T>* temp = top;
			top = top->getNext();
			delete temp;

			return true;
		}
	}
	T returner(int i) {
		int count = 0;
		Node<T>* current = top;
		while (count < i) {
			current = current->getNext();
			count++;
		}
		return current->getData();
	}
	T peek()
	{
		if (isEmpty())
			return NULL;
		else
			return top->getData();
	}
};

class Point;	// forward declaration
class Shape {
public:
	int depth;
public:
	Shape() {
		depth = 0;
	}
	Shape(int d) {
		if (d < 0)
			throw "invalid argument";
		else
			depth = d;
	}
	bool setDepth(int d) {
		if (d < 0)
			throw"invalid argument";
		else
			depth = d;
	}
	int getDepth() {
		return depth;
	}

	int dim() {

	}
	void translate(float x, float y) {

	}
	void rotate() {

	}
	void scale(float f) {

	}
	virtual bool contains(const Point& p) = 0;
};

class Point : public Shape {
public:
	float x;
	float y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(float x, float y, int d = 0) {
		this->x = x;
		this->y = y;
		Shape::depth = d;
	}
	int dim() {
		return 0;
	}
	float getX()const {
		return this->x;
	}
	float getY()const {
		return this->y;
	}
	void translate(float x, float y) {
		this->x += x;
		this->y += y;
	}
	void rotate() {

	}
	void scale(float f) {

	}
	void display() {
		cout << x << ',' << y << endl;
	}
	bool contains(const Point& p) {
		if (p.x == x && p.y == y)
			return true;
		return false;
	}
};


class LineSegment : public Shape {
public:
	void display() {
		cout << start.x << ',' << start.y << endl;
		cout << end.x << ',' << end.y << endl;
	}
	Point start;
	Point end;
	LineSegment() {

	}
	LineSegment(const Point& p, const Point& q) {
		if (p.x == q.x || p.y == q.y) {	// axis aligned check
			start = p;
			end = q;
		}
		else if (p.x != q.x && p.y != q.y)
			throw "Line segment is not axis aligned";
		if (p.depth != q.depth)
			throw "Depths of both points are not the same";
		Shape::depth = p.depth;// = q.depth;
		if (p.x == q.x && p.y == q.y)	// both points are on same point
			throw "Both points are on same point. Line segment cannot be formed.";

		//		if (p.y==q.y && p.x == q.x) // both points are on same point
	}
	//	int Shape::dim() {
	//		return 1;
	//	}
	void translate(float x, float y) {
		start.translate(x, y);
		end.translate(x, y);
	}
	void rotate() {
		Point center;
		center.x = end.x + start.x;
		center.x /= 2;

		center.y = end.y + start.y;
		center.y /= 2;

		if (start.x == end.x) {	// parallel to x axis
			start.x += center.y;
			end.x -= center.y;
			//			end.x *= -1;

			start.y = center.y;
			end.y = center.y;
		}
		else if (start.y == end.y) {	// parallel to y axis
			start.y += center.x;
			end.y -= center.x;
			//			end.y *= -1;

			start.x = center.x;
			end.x = center.x;
		}
	}
	float getXmin()const {
		return start.x;
	}
	float getXmax()const {
		return end.x;
	}
	float getYmin()const {
		if (start.y < end.y) return start.y;

		return end.y;
	}
	float getYmax()const {
		if (end.y > start.y) return end.y;

		return start.y;
	}
	float length()const {
		float len;
		if (start.x == end.x)	// line is vertical
			len = start.y - end.y;
		else if (start.y == end.y)	// line is horizontal
			len = start.x - end.x;

		if (len < 0)
			len *= -1;

		return len;
	}
	bool contains(const Point& p) {
		if (p.y == start.y)
		{
			if (p.x >= start.x && p.x <= end.x)
				return true;
		}
		else if (p.x == start.x) {
			if (p.y >= start.y && p.x <= end.x)
				return true;
		}
		return false;
	}
//	{
//		float greaterX, greaterY, smallerX, smallerY;
//		if (start.x > end.x)
//		{
//			greaterX = start.x;
//			smallerX = end.x;
//		}
//		else
//		{
//			greaterX = end.x;
//			smallerX = start.x;
//		}
//		if (start.y > end.y)
//		{
//			greaterY = start.y;
//			smallerY = end.y;
//		}
//		else {
//			greaterY = end.y;
//			smallerY = start.y;
//		}
//		if (start.x == end.x) {
//			if (p.x == start.x)
//			{
//				if (p.y >= smallerY && p.y <= greaterY)
//					return true;
//			}
//			else
//				return false;
//		}
//		if (start.y == end.y) {
//			if (p.y == start.y)
//			{
//				if (p.x >= smallerX && p.x <= greaterX)
//					return true;
//				else
//					return false;
//			}
//		}
//	}
	void scale(float f) {
		//		if (f<0) {
		//			throw "Invalid factor";
		//		}
		//		if (f<1) {
		//
		//		}
		//		else {
		//			Point center;
		//			center.x = end.x + start.x;
		//			center.x /= 2;
		//
		//			center.y = end.y + start.y;
		//			center.y/=2;
		//
		//			start.x = (start.x - end.x )/ f;
		//			}
	}
};

class TwoDShape : public Shape {
public:
	TwoDShape() :Shape() {

	}
	//	TwoDShape(const Point){
	//
	//	}
	TwoDShape(int d) {
		Shape::depth = d;
	}
	//	void translate(float x, float y) {
	//
	//	}
	//	int Shape::dim() {
	//		return 2;
	//	}
	float area() const {

	}
};

class Rectangle : public TwoDShape {
	Point p1;
	Point p2;
public:
	Rectangle() {

	}
	Rectangle(const Point& p, const Point& q) {
		if (p.x == q.x || p.y == q.y)
			throw "Rectangle cannot be formed";
		if (p.depth != q.depth)
			throw "depth should be same";
		p1 = p;
		p2 = q;
		//		Shape::depth = p.depth;

		//		throw std::invalid_argument(const std::string& what_arg)
	}
	//	float TwoDShape::area() {
	//		float area;
	////		area =
	//	}
	float area()const {
		float len, width;
		if (p1.x > p2.x)
			len = p1.x - p2.x;
		else
			len = p2.x - p1.x;

		if (len < 0)
			len *= -1;
		width = p1.y - p2.y;
		if (width < 0)
			width *= -1;

		return len * width;
	}
	float getXmin()const {
		if (p1.x < p2.x) return p1.x;

		return p2.x;
	}
	float getXmax()const {
		if (p1.x > p2.x) return p1.x;

		return p2.x;
	}
	float getYmin()const {
		if (p1.y < p2.y) return p1.y;

		return p2.y;
	}
	float getYmax()const {
		if (p1.y > p2.y) return p1.y;

		return p2.y;
	}
	void translate(float x, float y) {
		p1.translate(x, y);
		p2.translate(x, y);
	}
	void rotate() {
		Point center, s, e;
		center.x = p1.x + p2.x;
		center.x /= 2;
		center.y = p1.y + p2.y;
		center.y /= 2;

		s.x = p1.x;
		s.y = center.y;
		e.x = p2.x;
		e.y = center.y;

		LineSegment l;

		l.start = s;
		l.end = e;
		l.rotate();

		p1.x = p1.x + center.x - center.y;
		p1.y = p1.y + center.x - center.y;

		p2.x = p2.x - center.x + center.y;
		p2.y = p2.y - center.x + center.y;
	}
	void display() {
		cout << "Rectangle coordinates\n";
		cout << "(x1,y1) : " << p1.x << ',' << p1.y << endl;
		cout << "(x2,y2) : " << p2.x << ',' << p2.y << endl;
	}
	void scale(float f) {
		if (f < 0) {
			throw "Invalid factor";
		}
		Point center;
		center.x = p1.x + p2.x;
		center.x /= 2;
		center.y = p1.y + p2.y;
		center.y /= 2;

		if (f < 1) {
			p1.x = p1.x + center.x * (f);
			p1.y = p1.y - center.y * (f);
			p2.x = p2.x - center.x * (f);
			p2.y = p2.y + center.y * (f);
		}
		else {
			p1.x = p1.x - center.x * (f - 1);
			p1.y = p1.y + center.y * (f - 1);
			p2.x = p2.x + center.x * (f - 1);
			p2.y = p2.y - center.y * (f - 1);
		}
	}
	bool contains(const Point& p) {
		int greaterX, smallerX, greaterY, smallerY;
		if (p1.x > p2.x)
			greaterX = p1.x, smallerX=p2.x;
		else
			greaterX = p2.x, smallerX=p1.x;
		if (p1.y > p2.y)
			greaterY = p1.y, smallerY = p2.y;
		else
			greaterY = p2.y, smallerY = p1.y;

		if (p.x >= smallerX && p.x <= greaterX && p.y >= smallerY && p.y <=greaterY)
			return true;
		return false;
		/*		if (p2.x > p1.x && p2.y < p1.y) {

			if (p.x > p2.x || p.y < p2.y || p.x<p1.x || p.y>p1.y)
				return false;
		}
		else if (p2.x < p1.x && p2.y > p1.y) {
			if (p.x<p2.x || p.y > p2.y || p.x > p1.y || p.y < p1.y)
				return false;
		}
		else if (p2.y > p1.y) {
			if (p.y<p1.y && p.y>p2.y)
				return false;
		}
		else if (p2.x < p1.x) {
			if (p.x < p2.x || p.x>p1.x)
				return false;
		}*/
		return true;
	}
};

class Circle : public TwoDShape {
	Point center;
	float radius;
	const float PI = 3.1415;
public:
	Circle() {
		radius = 0;
	}
	Circle(const Point& c, float r) {
		//	Shape::depth = c.depth;
		center = c;
		if (r <= 0)
			throw "Circle cannot be formed";
		radius = r;
	}
	//	float TwoDShape::area();
	float getX()const {
		return this->center.x;
	}
	float getY()const {
		return this->center.y;
	}
	float getR()const {
		return this->radius;
	}
	void translate(float x, float y) {
		center.translate(x, y);
	}
	void rotate() {

	}
	void scale(float f) {
		if (f < 0) {
			throw "Invalid factor";
		}
		else {
			radius *= f;
		}
	}
	bool contains(const Point& p) {
		float distance = sqrt(((p.x - center.x) * (p.x - center.x)) + ((p.y - center.y) * (p.y - center.y)));
		if (distance < radius)
			return true;

		return false;
	}

	const Point& getCenter() const {
		return center;
	}

	void setCenter(const Point& center) {
		this->center = center;
	}

	float getRadius() const {
		return radius;
	}

	void setRadius(float radius) {
		this->radius = radius;
	}
	float area()const {
		return PI * radius * radius;
	}
	void display() {
		cout << "Circle coordinates : ";
		cout << "Center (x,y) : ";
		center.display();
		cout << "radius : "<< radius << endl;
	}
};
Stack <shared_ptr<Shape>> dstr;
class Scene {
	friend ostream& operator<<(ostream& out, const Scene& s);
public:
	static constexpr int WIDTH = 60;
	static constexpr int HEIGHT = 20;

	void addObject(shared_ptr<Shape> p) {
		dstr.push(p);
	}
	void setDrawDepth(int d) {

	}
};

bool check_if_Point_in_any_object_of_Stack(const Point & p) {
	for (int i = 0; i < dstr.length(); i++) {
		shared_ptr<Shape> ptr = dstr.returner(i);
		if (ptr->contains(p)) {
			return true;
		}
	}
	return false;
}
ostream& operator<<(ostream& out, const Scene& s) {
	for (int i = s.HEIGHT-1; i >= 0; i--) {	// y
		for (int j = 0; j < s.WIDTH; j++) {	// x
			Point point(j,i);
			if (check_if_Point_in_any_object_of_Stack(point))
			{
//				cout << j << i;	// coordinates
				cout << '*';
			}
			else
				cout << ' ';
		}
		cout << '\n';
	}
	return out;
}

int main() {
	//	Point p1(2, 8);
	//	Point p2(8, 4);
	//	Point p3(15, 9);
	//	shared_ptr<Shape> rp = make_shared<Rectangle>(p1, p2);
	//	shared_ptr<Shape> rp2 = make_shared<Circle>(p3, 4);
	//	Scene s;
	//	s.addObject(rp);
	//	s.addObject(rp2);
	//	cout << s;
	//	cout << rp->getDepth() << endl;
	//	return 0;

	Point p1(-1,-2);
	Point p2(10,-2);
	cout << p1.getX() << endl;

	LineSegment l(p1, p2);
	cout << l.getXmin() << " " << l.getXmax() << endl;
	cout << l.length() << endl;

	p2.translate(0,2);	// 10, 0
	Rectangle r(p1,p2);
	r.display();
	r.rotate();
	r.display();
	cout << r.getYmin() << " " << r.getYmax() << endl;
	cout << r.area() << endl;

	Point p3(1,2,3);
	Circle c(p3, 2);
	c.display();
	cout << c.area() << endl;
	c.scale(10);
	cout << c.area() << endl;
	c.display();

	auto pp1 = make_shared<Point>(0,0);
	auto pp2 = make_shared<Point>(0,19);
	auto pp3 = make_shared<Point>(59,19);
	auto pp4 = make_shared<Point>(59,0);
	auto pp5 = make_shared<Point>(55,19);
	auto pp6 = make_shared<Point>(30,0);

	auto lp = make_shared<LineSegment>(*pp2,*pp3);
	auto rp = make_shared<Rectangle>(*pp4,*pp5);
	auto cp = make_shared<Circle>(*pp6,10);

	Scene s;
	s.addObject(pp1);
	s.addObject(lp);
	s.addObject(rp);
	s.addObject(cp);
	cout << s;

	cout << endl;

	rp->display();
	rp->rotate();
	rp->display();
	cp->display();
	cp->translate(0,-5);
	cp->display();
	cout << s;
}

