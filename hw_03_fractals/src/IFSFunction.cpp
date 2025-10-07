#include "IFSFunction.h"



Point Spherical::operator()(const Point& P) const {
	float rsq = P.x * P.x + P.y * P.y;
	if (abs(rsq) < EPSILON){
		std::cout << "Epsilon" << std::endl;
		return P;
	}
	Point result(P.x / rsq, P.y / rsq);
	//std::cout << "rsq = " << rsq << " ";
	//std::cout << "result = " << result.x << ", " << result.y << std::endl;
	return result;
}

IFSFunctionSystem::IFSFunctionSystem(
		const std::vector<IFSFunction*>& functions,
		const std::vector<Color>& colors,
		std::vector<float>& weights
	) {
	// Set the funcs and colors, normalize the weights, and move on!
	_ifs_functions = functions;
	_colors = colors;
	_weights = weights;
	normalize(_weights);
}

void IFSFunctionSystem::normalize(std::vector<float>& vec){
	// TODO -> account for rotation weights having an even chance
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

int IFSFunctionSystem::get_random_weighted_index(){
	float randf = drand48();
	float weight_sum = 0.0f;
	for (int i = 0; i < _weights.size(); i++){
		weight_sum += _weights[i];
		if (randf <= weight_sum){
			return i;
		}
	}

	std::cout << "Random weight somehow didn't get picked. Should not happen" << std::endl;
	return _weights.size() - 1;
}
