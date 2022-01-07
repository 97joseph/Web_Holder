#include "Geometry.h"
using namespace std;

// ============ Shape class =================

Shape::Shape() {depth = 0;} // REMOVE ME

Shape::Shape(int d) {
	if (d < 0)
		cout << "wrong";//throw invalid_argument("depth cannot be less than zero");
	else
		depth = d;
	// IMPLEMENT ME
}

bool Shape::setDepth(int d) {
	// IMPLEMENT ME
//	return false; // dummy
	if (d < 0)
	{
		//throw invalid_argument("depth cannot be less than zero");
		return false;
	}
	else
		depth = d;
	return true;
}

int Shape::getDepth()const {
//	 IMPLEMENT ME
//	return -999; // dummy
	return depth;
}
//int Shape::dim()const{return 999;}
//int Shape::dim()const{};//{}// {
//	// IMPLEMENT ME
//	return -999; // dummy
//}

//void Shape::translate(float x, float y) {
//	// IMPLEMENT ME
//	cout << "I'm in shape" << endl;
//}

//void Shape::rotate() {
//	// IMPLEMENT ME
//}
//
//void Shape::scale(float f) {
//	// IMPLEMENT ME
//}

//bool Shape::contains(const Point& p);
//const {
//	// IMPLEMENT ME
//	return false; // dummy
//}

// =============== Point class ================
Point::Point(){
	x=0;
	y=0;
	depth=0;
}
Point::Point(float x, float y) {
	this->x = x;
	this->y = y;
	depth = 0;
}
Point::Point(float x, float y, int d) {
	// IMPLEMENT ME
	if (d<0)
		throw invalid_argument("invalid depth");
	this->x = x;
	this->y = y;
	depth = d;
}

float Point::getX() const {
	// IMPLEMENT ME
//	return -999; // dummy
	return this->x;
}

float Point::getY() const {
	// IMPLEMENT ME
//	return -999; // dummy
	return this->y;
}
int Point::dim() const{
	return 0;
}
void Point::translate(float x, float y)
{
	this->x += x;
	this->y += y;
//	cout << "x : " << this->x << "y: " << this->y;
}
void Point::rotate(){}
void Point::scale(float f){
	if (f<=0)
		throw invalid_argument("invalid factor");
}
bool Point::contains(const Point&p){
	if (p.x == x && p.y == y)
		return true;
	return false;
}

	bool Point::setDepth(int d) {
		if (d < 0)
		{
			return false;
//			throw"invalid argument";
		}
		else
			depth = d;
		return true;
	}

	int Point::getDepth(){
		return depth;
	}
//	int Point::dim(){return 0;}
// =========== LineSegment class ==============

LineSegment::LineSegment(const Point& p, const Point& q) {
	// IMPLEMENT ME
	if (p.x != q.x && p.y != q.y)
		throw invalid_argument("Line segment is not axis aligned");
	if (p.depth != q.depth)
		throw invalid_argument("Depths of both points are not the same");
	this->depth = p.depth;// = q.depth;
	if (p.x == q.x && p.y == q.y)	// both points are on same point
		throw invalid_argument("Both points are on same point. Line segment cannot be formed.");
	if (p.x == q.x || p.y == q.y) {	// axis aligned check
		start = p;
		end = q;
	}
	//		if (p.y==q.y && p.x == q.x) // both points are on same point
}
int LineSegment::dim()const{return 1;}

float LineSegment::getXmin() const {
	// IMPLEMENT ME
//	return -999; // dummy
	return start.x;
}

float LineSegment::getXmax() const {
	// IMPLEMENT ME
//	return -999; // dummy
	return end.x;
}

float LineSegment::getYmin() const {
	// IMPLEMENT ME
//	return -999; // dummy
	if (start.y < end.y) return start.y;

	return end.y;
}

float LineSegment::getYmax() const {
	// IMPLEMENT ME
//	return -999; // dummy
	if (end.y > start.y) return end.y;

	return start.y;
}

float LineSegment::length() const {
	// IMPLEMENT ME
//	return -999; // dummy
	float len;
	if (start.x == end.x)	// line is vertical
		len = start.y - end.y;
	else if (start.y == end.y)	// line is horizontal
		len = start.x - end.x;

	if (len < 0)
		len *= -1;

	return len;
}
//void LineSegment::translate(float x, float y){
//	start.translate(x, y);
//	end.translate(x, y);
//}
void LineSegment::rotate(){
		float diff;
		Point center;
		center.x = end.x + start.x;
		center.x /= 2;

		center.y = end.y + start.y;
		center.y /= 2;
		//center.display();

		if (start.x == end.x) {	// parallel to x axis
			diff = center.y-start.y;
			start.x = start.x - diff;
			end.x = end.x + diff;
			//			end.x *= -1;

			start.y = center.y;
			end.y = center.y;
		}
		else if (start.y == end.y) {	// parallel to y axis
			diff = center.x-start.x;
			start.y = start.y - diff;
			end.y = end.y + diff;
			//			end.y *= -1;

			start.x = center.x;
			end.x = center.x;
		}
}
bool LineSegment::contains(const Point& p){
		float greaterX, smallerX, greaterY, smallerY;
		if (start.x > end.x){
			greaterX = start.x;
			smallerX = end.x;
		}
		else{
			greaterX = end.x;
			smallerX = start.x;
		}
		if (start.y > end.y){
			greaterY = start.y;
			smallerY = end.y;
		}
		else{
			greaterY = end.y;
			smallerY = start.y;
		}

		if (p.y == start.y)
		{
			if (p.x >= smallerX && p.x <= greaterX)
				return true;
		}
		else if (p.x == start.x) {
			if (p.y >= smallerY && p.y <=greaterY)
				return true;
		}
		return false;
}
void LineSegment::scale(float f){
//	cout << start.x  << ',' << end.x << '\n';
//	cout << start.y << ',' << end.y << endl;
		if (f<=0) {
			throw invalid_argument("Invalid factor");
		}
		Point center;
		center.x = end.x + start.x;
		center.x /= 2;

		center.y = end.y + start.y;
		center.y/=2;
		if (f<1) {
			int len;
			if (end.x > start.x)
				len = end.x - start.x;
			else
				len = start.x - end.x;

			len *= f;
			if (start.y==end.y) {
				start.x = center.x-len/2;
				end.x = center.x+len/2;
			}
			else if (start.x==end.x) {
				start.y = center.y-len/2;
				end.y = center.y+len/2;
			}
		}
		else {
	//		center.display();
			if (start.y == end.y) {
				int diff;
				diff = center.x - start.x;
				start.x = end.x = 0;

				start.x -= center.x * f - diff;
				end.x += center.x * f+diff;
			}
			else if (start.x == end.x)
			{
				int len, diff;
//				diff = center.y - start.y;
				len = end.y - start.y;
				start.y = end.y = 0;
				len*=f;
				len/=2;
				start.y -= len-center.y;
				end.y += len+center.y;
			}
	//			start.x = (start.x - end.x )/ f;
		}
//		cout << start.x  << ',' << end.x << '\n';
//		cout << start.y << ',' << end.y << endl;
}

//int LineSegment::getDepth() {
//	return depth;
//}

// ============ TwoDShape class ================

TwoDShape::TwoDShape():Shape(){} // REMOVE ME

TwoDShape::TwoDShape(int d) {
	// IMPLEMENT ME
	if (d < 0)
		throw invalid_argument("depth cannot be less than zero");
	else
		this->depth = d;
}

//float TwoDShape::area() const
//{
	// IMPLEMENT ME
//	return -999; // dummy
//}

// ============== Rectangle class ================
Rectangle::Rectangle(){}
Rectangle::Rectangle(const Point& p, const Point& q) {
	// IMPLEMENT ME
	if (p.x == q.x || p.y == q.y)
		throw invalid_argument("Rectangle cannot be formed");
	if (p.depth != q.depth)
		throw invalid_argument("depth should be same");
	p1 = p;
	p2 = q;
	depth = p.depth;
}

int Rectangle::dim()const{return 2;}

float Rectangle::getXmin() const {
	// IMPLEMENT ME
//	return -999; // dummy
	if (p1.x < p2.x) return p1.x;

	return p2.x;
}

float Rectangle::getYmin() const {
	// IMPLEMENT ME
//	return -999; // dummy
	if (p1.y < p2.y) return p1.y;

	return p2.y;
}

//int Rectangle::getDepth() const {
//	return this->depth;
//}

float Rectangle::getXmax() const {
	// IMPLEMENT ME
//	return -999; // dummy
	if (p1.x > p2.x) return p1.x;

	return p2.x;
}

float Rectangle::getYmax() const {
	// IMPLEMENT ME
//	return -999; // dummy
	if (p1.y > p2.y) return p1.y;

	return p2.y;
}
//void Rectangle::translate(float x, float y){
//	p1.translate(x, y);
//	p2.translate(x, y);
//}
void Rectangle::rotate(){
	Point center, s, e;
	center.x = p1.x + p2.x;
	center.x /= 2;
	center.y = p1.y + p2.y;
	center.y /= 2;

	float x = p1.x - center.x;
	float y = p1.y - center.y;

	p1.x = center.x - y;
	p1.y = center.y + x;

	x = p2.x - center.x;
	y = p2.y - center.y;

	p2.x = center.x - y;
	p2.y = center.y + x;
}
void Rectangle::scale(float f){
		if (f <= 0) {
			throw invalid_argument("Invalid factor");
		}
		Point center;
		center.x = p1.x + p2.x;
		center.x /= 2;
		center.y = p1.y + p2.y;
		center.y /= 2;


			float diff1, diff2;
			if (p1.x > p2.x)
			{
//				len = p1.x - p2.x;
				diff1 = center.x - p2.x;
			}
			else
			{
//				len = p2.x - p1.x;
				diff1 = center.x - p1.x;
			}
			if (p1.y > p2.y)
			{
//				width = p1.y - p2.y;
				diff2 = center.y - p2.y;
			}
			else
			{
//				width = p2.y - p1.y;
				diff2 = center.y - p1.y;
			}
			diff1*=f;
			diff2*=f;
			if (p1.x < p2.x)
			{
				p1.x = center.x-diff1;
				p2.x = center.x+diff1;
			}
			else {
				p2.x = center.x-diff1;
				p1.x = center.x+diff1;
			}
			if (p1.y < p2.y)
			{
				p1.y = center.y-diff2;
				p2.y = center.y+diff2;
			}
			else {
				p2.y = center.y-diff2;
				p1.y = center.y+diff2;
			}
}
bool Rectangle::contains(const Point &p) {
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
}
float Rectangle::area()const{
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

int Rectangle::getDepth() const{
	return depth;
}

// ================== Circle class ===================

Circle::Circle(const Point& c, float r) {
	// IMPLEMENT ME
		center = c;
		if (r == 0)
			throw invalid_argument("Circle cannot be formed");
		if (r < 0)
			throw invalid_argument("Circle cannot be formed");			
		radius = r;
		depth = c.depth;
}

int Circle::dim()const{return 2;}

float Circle::getX() const {
	// IMPLEMENT ME
//	return -999; // dummy
	return center.x;
}

float Circle::getY() const {
	// IMPLEMENT ME
//	return -999; // dummy
	return center.y;
}

float Circle::getR() const {
	// IMPLEMENT ME
//	return -999; // dummy
	return radius;
}

//void Circle::translate(float x, float y){
//	center.translate(x, y);
//}
void Circle::rotate(){}
void Circle::scale(float f){
	if (f <= 0) {
		throw invalid_argument("Invalid factor");
	}
	else {
		radius *= f;
	}
}
bool Circle::contains(const Point&p){
	float distance = sqrt(((p.x - center.x) * (p.x - center.x)) + ((p.y - center.y) * (p.y - center.y)));
	if (distance <= radius)
		return true;

	return false;
}
float Circle::area()const{
	return PI * radius * radius;
}

//int Circle::getDepth(){
//	return depth;
//}

// ================= Scene class ===================

Scene::Scene() {
//	Stack <shared_ptr<Shape>> dstr;	// data structure
	dstr = new Stack<shared_ptr<Shape>>;
	// IMPLEMENT ME
	for (int i=HEIGHT-1; i>=0; i--) {
		for (int j=0; j<WIDTH; j++){
			cout << ' ';
		}
		cout << '\n';
	}
	drawDepth = 100;
}

void Scene::addObject(std::shared_ptr<Shape> ptr) {
	// IMPLEMENT ME
	dstr->push(ptr);
//	setDrawDepth(ptr->getDepth());
}

void Scene::setDrawDepth(int depth) {
//	// IMPLEMENT ME
//	if (depth > drawDepth)
		drawDepth = depth;
}
bool check_if_Point_in_any_object_of_Stack(const Point & p, Stack<shared_ptr<Shape>>* dstr, int d){
	for (int i = 0; i < dstr->length(); i++) {
		shared_ptr<Shape> ptr = dstr->returner(i);
		if (ptr->contains(p) && ptr->getDepth() <= d) {
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& out, const Scene& s) {
	// IMPLEMENT ME
	for (int i = s.HEIGHT-1; i >= 0; i--) {	// y
		for (int j = 0; j < s.WIDTH; j++) {	// x
			Point point(j,i);
			if (check_if_Point_in_any_object_of_Stack(point, s.dstr, s.drawDepth))// && s.dstr->getTop().getDepth() < s.drawDepth)
			{
//				cout << j << i;	// coordinates
				out << '*';
			}
			else
				out << ' ';
		}
		out << '\n';
	}
	return out;
}



