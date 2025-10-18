#ifndef _IFS_FUNCTION_H
#define _IFS_FUNCTION_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <complex>
#include <iostream>


#include <image_data.h>
#include <color.h>

const float EPSILON = 0.000001;
const float PI = 3.14159265358979;
const float PI_OVER_TWO = PI / 2;

struct Point{
	double x, y;
	Point(double new_x, double new_y) { x = new_x; y = new_y; } 
	Point() {x = 0.0, y = 0.0; };

	void operator*=(double val) { x *= val, y *= val; }
	void operator+=(const Point& other) { x += other.x, y += other.y; }

	double magnitude_sq() {return x*x + y*y; }

};


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

	Point operator()(const Point& P) const;
};

class Sinusoidal : public IFSFunction {
public:
	Sinusoidal() {}
	~Sinusoidal() {}

	Point operator()(const Point& P) const;
};

class Scale : public IFSFunction {
public:
	Scale(float x_scale, float y_scale) { _x_scale = x_scale, _y_scale = y_scale ; }
	~Scale() {}

	Point operator()(const Point& P) const;

private:
	float _x_scale;
	float _y_scale;
};

class JuliaIterations : public IFSFunction {
public:	
	JuliaIterations(const Point& complex_center, int iters, int cycles);
	~JuliaIterations() {}

	Point operator()(const Point& P) const;

private:
	Point _complex_center;
	int _iterations;
	int _cycles;
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
	Rotation(float radians);
	~Rotation() {}

	float get_radians() const { return _radians; }

	Point operator()(const Point& P) const;

private:
	// TO DO -> Cache the rotation matrix when the radians are set
	// Might be a good time to include a matrix class?
	float _rotation_matrix_2D[4];
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
		IFSFunction* final_function,
		int width,
		int height
	);

	~IFSFunctionSystem() {};
	void fractal_frame(int iter);

	int get_random_weighted_index(const std::vector<float>& weights);
	IFSFunction* get_ifs_function(int index) const { return _ifs_functions[index]; }
	SymmetryIFS* get_symmetry_function(int index) const { return _symmetry_functions[index]; }
	const Color& get_color(int index) const { return _colors[index]; }
	const ImageData& get_image() const {return img; }

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