#ifndef _IFS_FUNCTION_H
#define _IFS_FUNCTION_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <iostream>

#include <image_data.h>

const float EPSILON = 0.000001;

struct Point{
	double x, y;
	Point(double new_x, double new_y) { x = new_x; y = new_y; } 
	Point();
};

struct Color{
	float r, g, b = 0.0f;
	Color() {r = 0.0f; g = 0.0f; b = 0.0f; };
	Color(float r_, float g_, float b_) {r = r_; g = g_; b = b_; }
	Color(float val) {r = val; g = val; b = val; }

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
};

class Spherical : public IFSFunction {
public:
	Spherical() {}
	~Spherical() {}

	Point operator()(const Point&P) const;
};

// Symmetry IFS functions are different so we can skip adding the color when a 
// symmetry operation is performed. 
class SymmetryIFS : public IFSFunction {
public:
	SymmetryIFS() {}
	~SymmetryIFS() {}
};

class Rotation : public SymmetryIFS{
public:
	Rotation(float radians) {_radians = radians; }
	~Rotation() {}

	float get_radians() const { return _radians; }

private:
	float _radians;

};

class IFSFunctionSystem{
public:
	IFSFunctionSystem() {}
	IFSFunctionSystem(
		const std::vector<IFSFunction*>& functions,
		std::vector<float>& weights,
		const std::vector<Color>& colors,
		const std::vector<SymmetryIFS*>& symmetry_functions,
		std::vector<float>& symmetry_weights,
		int width,
		int height
	);

	~IFSFunctionSystem() {};
	void fractal_frame(int iter);

	int get_random_weighted_index(const std::vector<float>& weights);
	IFSFunction* get_ifs_function(int index) const { return _ifs_functions[index]; }
	SymmetryIFS* get_symmetry_function(int index) const { return _symmetry_functions[index]; }
	const Color& get_color(int index) const { return _colors[index]; }

	void normalize(std::vector<float>& vec);

	// for RGBA
	const int N_CHANNELS = 4;

private:

	ImageData img;

	std::vector<IFSFunction*> _ifs_functions;
	IFSFunction* _final_function;
	std::vector<SymmetryIFS*> _symmetry_functions;
	std::vector<Color> _colors;
	std::vector<float> _weights;
	std::vector<float> _symmetry_weights;
};



#endif