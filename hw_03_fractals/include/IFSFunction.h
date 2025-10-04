#ifndef _IFS_FUNCTION_H
#define _IFS_FUNCTION_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <iostream>

const float EPSILON = 0.000001;

struct Point{
	float x, y;
	Point(float new_x, float new_y) { x = new_x; y = new_y; } 
	Point();
};

struct Color{
	float r, g, b = 0.0f;
	Color(float r_, float g_, float b_) {r = r_; g = g_; b = b_; }
	Color() {r = 0.0f; g = 0.0f; b = 0.0f; };

	// overload the plus operator
	Color operator+(const Color& other) const {
		return Color(r + other.r, g + other.g, b + other.b);
	}
	Color operator/(const Color& other) const {
		return Color(r / other.r, g / other.g, b / other.b);
	}

	Color operator/(const float f) const {
		return Color(r / f, g / f, b / f);
	}
};


// TO DO -> add a center or x/y translations along with a time??

class IFSFunction {
public:
	IFSFunction(){};
	virtual ~IFSFunction() = default;

	virtual Point operator()(const Point& P) const = 0;
	Color get_color() const {return _color; }
	void set_color(Color color) {_color = color; }

protected:
	Color _color;
};

class Spherical : public IFSFunction {
public:
	Spherical() {};
	~Spherical() {};

	Point operator()(const Point&P) const;
};


void apply_random_func(int iters, std::vector<IFSFunction*>& function_ptrs, Point& point, Color& color);
void apply_random_func(int iters, std::vector<IFSFunction*>& function_ptrs, std::vector<Color>& colors, Point& point, Color& color);

#endif