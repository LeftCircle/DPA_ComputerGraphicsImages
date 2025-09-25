#ifndef STENCIL_H
#define STENCIL_H

#include <memory>

class Stencil {
public:
    Stencil(int _half_width=1);
    ~Stencil();

    int get_halfwidth() const { return _half_width; };
    float& operator() (int i, int j);
    const float& operator() (int i, int j) const;
    

private:
    int _half_width;
    std::unique_ptr<float[]> _stencil_values;

};


#endif