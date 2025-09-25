#include "stencil.h"

Stencil::Stencil(int half_width) {
    resize(half_width)
}

Stencil::~Stencil() {};

void Stencil::resize(int new_half_width){
    _half_width = new_half_width;
    int stencil_width = 2 * new_half_width + 1;
    int stencil_size = stencil_width * stencil_width;
    _stencil_values = std::make_unique<float[]>(stencil_size);
}