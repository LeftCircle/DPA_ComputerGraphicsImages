#include "IFSFunction.h"



Point Spherical::operator()(const Point& P) const {
	double rsq = P.x * P.x + P.y * P.y;
	if (abs(rsq) < EPSILON){
		std::cout << "Epsilon" << std::endl;
		return P;
	}
	Point result(P.x / rsq, P.y / rsq);
	//std::cout << "rsq = " << rsq << " ";
	//std::cout << "result = " << result.x << ", " << result.y << std::endl;
	return result;
}

Point Scale::operator()(const Point& P) const {
	Point new_p(0.0, 0.0);
	new_p.x = P.x * _x_scale;
	new_p.y = P.x * _y_scale;
	return new_p;
}

JuliaSet::JuliaSet(const Point& center, int iters, int cycles){
	 _iterations = iters, _cycles = std::max(cycles, 2);
	 _center.x = center.x;
	 _center.y = center.y; 
}

Point JuliaSet::operator()(const Point& P) const {
	std::complex<double> Pc(P.x, P.y);
	std::complex<double> cc(_center.x, _center.y);

	for (int i = 0; i < _iterations; i++){
		std::complex<double> temp = Pc;
		for (int c = 1; c < _cycles; c++){
			temp = temp * Pc;
		}
		Pc = temp + cc;
	}

	Point p_out(Pc.real(), Pc.imag());
	return p_out;
}

Rotation::Rotation(float radians){
	_radians = radians;
	float cos_r = cos(radians);
	float sin_r = sin(radians);
	_rotation_matrix_2D[0] = cos_r;
	_rotation_matrix_2D[1] = -sin_r;
	_rotation_matrix_2D[2] = sin_r;
	_rotation_matrix_2D[3] = cos_r;
}

Point Rotation::operator()(const Point& P) const {
	// Rotates the point by _radians.
	Point new_p(0.0, 0.0);
	new_p.x = _rotation_matrix_2D[0] * P.x + _rotation_matrix_2D[1] * P.y;
	new_p.y = _rotation_matrix_2D[2] * P.x + _rotation_matrix_2D[3] * P.y;
	return new_p;
}

Point Sinusoidal::operator()(const Point& P) const {
	Point new_p(0.0, 0.0);
	new_p.x = sin(P.x);
	new_p.y = sin(P.y);
	return new_p;
}

IFSFunctionSystem::IFSFunctionSystem(
		const std::vector<IFSFunction*>& functions,
		std::vector<float>& weights,
		const std::vector<Color>& colors,
		const std::vector<SymmetryIFS*>& symmetry_functions,
		std::vector<float>& symmetry_weights,
		IFSFunction* final_function,
		int width,
		int height
	) {
	// Set the funcs and colors, normalize the weights, and move on!
	_ifs_functions = functions;
	_colors = colors;
	_weights = weights;
	_symmetry_functions = symmetry_functions;
	_symmetry_weights = symmetry_weights;
	_final_function = final_function;
	normalize(_symmetry_weights);
	normalize(_weights);
	img.set_dimensions(width, height, N_CHANNELS);
}

void IFSFunctionSystem::normalize(std::vector<float>& vec){
	float sum = 0.0;
	float n_elements = vec.size();
	for (int i = 0; i < n_elements; i++){
		sum += vec[i];
	}
	float one_over_sum = 1.0 / sum;
	for (int i = 0; i < n_elements; i++){
		vec[i] *= one_over_sum;
	}
}

int IFSFunctionSystem::get_random_weighted_index(const std::vector<float>& weights){
	float randf = drand48();
	float weight_sum = 0.0f;
	for (int i = 0; i < weights.size(); i++){
		weight_sum += weights[i];
		if (randf <= weight_sum){
			return i;
		}
	}

	std::cout << "Random weight somehow didn't get picked. Should not happen" << std::endl;
	return weights.size() - 1;
}


void IFSFunctionSystem::fractal_frame(int iters){
	// for each iteration, roll a random check to see if we pull a 
	// IF or a Symmetry function. Then apply to the rand point
	img.set_pixel_values(0.0f);
	Point p(double(2 * drand48() - 1), double(2 * drand48() - 1));
	int width = img.get_width();
	int height = img.get_height();
	std::vector<float> rgb(3, 0.0f);

	// Alpha channel will contain hit count for now
	int rand_index;
	IFSFunction* ifs;
	SymmetryIFS* sym;

	for (int i = 1; i <= iters; i++){
		// Roll for symmetry func
		if (drand48() < 0.5){
			// symmetry!
			rand_index = get_random_weighted_index(_symmetry_weights);
			sym = get_symmetry_function(rand_index);
			p = (*sym)(p);
		} else{
			// Update p and also adjust the color
			rand_index = get_random_weighted_index(_weights);
			ifs = get_ifs_function(rand_index);
			const Color& c = get_color(rand_index);
			p = (*ifs)(p);

			// TO DO -> Add a final function that is the same every time?
			p = (*_final_function)(p);

			// Now add values to the img
			int xp = int(((p.x + 1.0f) / 2.0f) * width);
			int yp = int(((p.y + 1.0f) / 2.0f) * height);
			if (xp < 0 || xp >= width || yp < 0 || yp >= height){
				continue;
			} else {
				// add values to the pixel
				// TO DO -> Apply less value at the start? 
				img.add_values(xp, yp, c.r, c.g, c.b, 1.0f);
				//std::cout << "Drawing to pixel " << xp << " " << yp << " " << color.r << " " << color.g << " " << color.b <<  std::endl;
			}	
		}

		// Now we have to do our post processing and adjust the color and alpha parameters
		// based off of log(alpha) / alpha
		// #pragma omp parallel for
		// for (int j = 0; j < img.get_height(); j++){
		// 	for (int i = 0; i < img.get_width(); i++){
		// 		float a = img.get_pixel_value(i, j, 3);
		// 		if (a == 0){
		// 			continue;
		// 		}
		// 		float loga_over_a = log(a) / a;
		// 		std::cout << "log a over a = " << loga_over_a << " a = " << a << std::endl;
		// 		img.scale_pixel_values(i, j, loga_over_a);
		// 	}
		// }
	}
}