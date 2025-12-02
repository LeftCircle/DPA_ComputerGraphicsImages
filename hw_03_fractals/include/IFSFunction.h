#ifndef _IFS_FUNCTION_H
#define _IFS_FUNCTION_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <complex>
#include <iostream>
//#include <Eigen/Core>
//#include <Eigen/Dense>


#include <image_data.h>
#include <color.h>
#include <point.h>

const float EPSILON = 0.000001;
const float PI = 3.14159265358979;
const double PId = 3.14159265358979323846;
const float PI_OVER_TWO = PI / 2;

//  Ω
inline float zero_or_pi() { return (drand48() < 0.5) ? 0.0 : PI; }
// Λ
inline float neg_one_or_one() { return (drand48() < 0.5) ? -1.0 : 1.0; }
// Ψ
inline float uniform_0_1() { return static_cast<float>(drand48()); }
inline float trunc(float x) { return (x >= 0.0f) ? std::floor(x) : std::ceil(x); }
inline float decimal_part(float x) { return x - trunc(x); }


class IFSFunction {
public:
	IFSFunction() {}
	virtual ~IFSFunction() = default;

	virtual Point operator()(const Point& P) const = 0;
};

class FlameIFSFunction : public IFSFunction {
public:
	// FlameIFSFunction() {  _homogeneous_transform << 1.0, 0.0, 0.0,
    //                           0.0, 1.0, 0.0,
    //                           0.0, 0.0, 1.0;
	// }
	FlameIFSFunction() {}
	~FlameIFSFunction() {}

	virtual Point operator()(const Point& P) const = 0;
	//const Eigen::Matrix3d& get_trans_matrix() const { return _homogeneous_transform; }

	//void set_trans_matrix(double m00, double m01, double m02, double m10, double m11, double m12);
	//void add_rotation(double radians);

	//Point get_point_in_local_space(const Point& P);
	//Point convert_point_to_global_space(const Point& P);

protected:
	//Eigen::Matrix3d _homogeneous_transform;
	//Eigen::Matrix3d _inverse_transf;

};

class Randomize : public FlameIFSFunction{
public:
	Randomize() {}
	~Randomize() {}

	Point operator()(const Point& P) const { return Point(double(2 * drand48() - 1), double(2 * drand48() - 1)); }
};

class Sinusoidal : public FlameIFSFunction {
public:
	Sinusoidal() {}
	~Sinusoidal() {}

	Point operator()(const Point& P) const;
};

class Linear : public FlameIFSFunction {
public:
	Linear() : _scale(1.0, 1.0) {}
	Linear(float x_scale = 1.0, float y_scale = 1.0) : _scale(x_scale, y_scale) {}
	~Linear() {}
	Point operator()(const Point& P) const;
private:
	Point _scale;
};

class Truncate : public FlameIFSFunction {
public:
	Truncate() {}
	~Truncate() {}

	Point operator()(const Point& P) const { return Point(decimal_part(P.x), decimal_part(P.y)); };
};

class Spherical : public FlameIFSFunction {
public:
	Spherical() : _center(0.0, 0.0), _scale(1.0, 1.0) {}
	Spherical(const Point& center, const Point& scale) : _center(center), _scale(scale) {}
	Spherical(double center_x, double center_y, double scale_x, double scale_y) 
		: _center(Point(center_x, center_y)), _scale(Point(scale_x, scale_y)) {}
	~Spherical() {}

	Point operator()(const Point& P) const;

private:
	Point _center;
	Point _scale;
};

class Handkerchief : public FlameIFSFunction {
public:
	Handkerchief() {}
	~Handkerchief() {}
	Point operator()(const Point& P) const;
};


class JuliaIterations : public FlameIFSFunction {
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

class negate_x : public SymmetryIFS {
public:
	negate_x() {}
	~negate_x() {}

	Point operator()(const Point& P) const;
};

class IFSFunctionSystem{
public:
	IFSFunctionSystem() {}
	IFSFunctionSystem(
		const std::vector<FlameIFSFunction*>& functions,
		std::vector<float>& weights,
		const std::vector<Color>& colors,
		const std::vector<SymmetryIFS*>& symmetry_functions,
		std::vector<float>& symmetry_weights,
		FlameIFSFunction* final_function,
		int width,
		int height
	);

	~IFSFunctionSystem() {};
	void fractal_frame(int iter);

	int get_random_weighted_index(const std::vector<float>& weights);
	FlameIFSFunction* get_ifs_function(int index) const { return _ifs_functions[index]; }
	SymmetryIFS* get_symmetry_function(int index) const { return _symmetry_functions[index]; }
	const Color& get_color(int index) const { return _colors[index]; }
	const ImageData& get_image() const { return img; }

	void normalize(std::vector<float>& vec);

	// for RGBA
	const int N_CHANNELS = 4;
	bool has_symmetry() { return _symmetry_functions.size() > 0; }

private:

	ImageData img;

	std::vector<FlameIFSFunction*>_ifs_functions;
	FlameIFSFunction* _final_function;
	std::vector<SymmetryIFS*> _symmetry_functions;
	std::vector<Color> _colors;
	std::vector<float> _weights;
	std::vector<float> _symmetry_weights;

	bool _are_inputs_correct() const;
};



#endif