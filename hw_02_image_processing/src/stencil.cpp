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
	_two_half_width_plus_one = 2 * new_half_width + 1; 
	randomize_values();
}

void Stencil::randomize_values(float lower_bound, float upper_bound) {
	int n_elements = get_n_elements();
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(lower_bound, upper_bound);

	float sum_of_all_but_center = 0.0f;
	for (int i = 0; i < n_elements; i++) {
		// skip the center element
		if (i == (n_elements / 2)) {
			continue;
		} 
		float rand_val = distribution(generator);
		_stencil_values[i] = 0.0f;
		sum_of_all_but_center += 0.0f;
	}
	_stencil_values[n_elements / 2] = 1.0f - sum_of_all_but_center;
	std::cout << "Lower bound: " << lower_bound << ", Upper bound: " << upper_bound << std::endl;
	std::cout << "Center value adjusted to: " << _stencil_values[n_elements / 2] << std::endl;
	print_stencil();
	int sum_check = 0;
	for (int i = 0; i < n_elements; i++) {
		sum_check += _stencil_values[i];
	}
	std::cout << "Sum check (should be 0.0): " << sum_check << std::endl;
}

float& Stencil::get_value_offset_from_center(int x_offset, int y_offset) {
	int center_index = _half_width + _half_width * _two_half_width_plus_one;
	int target_index = center_index + x_offset + y_offset * _two_half_width_plus_one;
	return _stencil_values[target_index];
}

float& Stencil::operator() (int i, int j) {
	return _stencil_values[i + j * _two_half_width_plus_one];
}

const float& Stencil::operator() (int i, int j) const {
	return _stencil_values[i + j * _two_half_width_plus_one];
}

void Stencil::print_stencil() const {
	for (int j = 0; j < _two_half_width_plus_one; j++) {
		for (int i = 0; i < _two_half_width_plus_one; i++) {
			std::cout << (*this)(i, j) << " ";
		}
		std::cout << std::endl;
	}
}