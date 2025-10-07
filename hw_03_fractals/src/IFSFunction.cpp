#include "IFSFunction.h"



Point Spherical::operator()(const Point& P) const {
	double rsq = P.x * P.x + P.y * P.y;
	if (abs(rsq) < EPSILON){
		std::cout << "Epsilon" << std::endl;
		return P;
	}
	Point result(P.x / rsq, P.y / rsq);
	std::cout << "rsq = " << rsq << " ";
	std::cout << "result = " << result.x << ", " << result.y << std::endl;
	return result;
}

IFSFunctionSystem::IFSFunctionSystem(
		const std::vector<IFSFunction*>& functions,
		std::vector<float>& weights,
		const std::vector<Color>& colors,
		const std::vector<SymmetryIFS*>& symmetry_functions,
		std::vector<float>& symmetry_weights,
		int width,
		int height
	) {
	// Set the funcs and colors, normalize the weights, and move on!
	_ifs_functions = functions;
	_colors = colors;
	_weights = weights;
	_symmetry_functions = symmetry_functions;
	_symmetry_weights = symmetry_weights;
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
	int n_colors = _colors.size();
	Point p(double(2 * drand48() - 1), double(2 * drand48() - 1));
	Color color(0.0f, 0.0f, 0.0f);
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

			// Now add values to the img
			int xp = int(((p.x + 1.0f) / 2.0f) * width);
			int yp = int(((p.y + 1.0f) / 2.0f) * height);
			if (xp < 0 || xp >= width || yp < 0 || yp >= height){
				continue;
			} else {
				// add values to the pixel
				img.add_values(xp, yp, color.r, color.g, color.b, 1.0f);
			}	
		}

		// Now we have to do our post processing and adjust the color and alpha parameters
		// based off of log(alpha) / alpha
		#pragma omp parallel for
		for (int j = 0; j < img.get_height(); j++){
			float loga_over_a = 0.0f;
			float a;
			for (int i = 0; i < img.get_width(); i++){
				float a = img.get_pixel_value(i, j, 3);
				loga_over_a = log(a) / a;
				img.scale_pixel_values(i, j, loga_over_a);
			}
		}
	}
}