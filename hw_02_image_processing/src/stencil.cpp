#include "stencil.h"

Stencil::Stencil(int half_width) {
	resize(half_width);
}

Stencil::~Stencil() {};

void Stencil::resize(int new_half_width){
	_half_width = new_half_width;
	int stencil_width = 2 * new_half_width + 1;
	int stencil_size = stencil_width * stencil_width;
	_stencil_values = std::make_unique<float[]>(stencil_size);
	randomize_values();
}

void Stencil::randomize_values(float lower_bound, float upper_bound) {
	int n_elements = get_n_elements();
	std::default_random_engine generator;
	std::uniform_int_distribution<float> distribution(lower_bound, upper_bound);

	float sum_of_all_but_center = 0.0f;
	for (int i = 0; i < n_elements; i++) {
		// skip the center element
		if (i == (n_elements / 2)) {
			continue;
		} 
		float rand_val = distribution(generator);
		_stencil_values[i] = rand_val;
		sum_of_all_but_center += rand_val;
	}
	_stencil_values[n_elements / 2] = 1.0f - sum_of_all_but_center;
}