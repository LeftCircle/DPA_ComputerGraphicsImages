#ifndef STATS_H
#define STATS_H

#include <vector>

template<typename T>
std::vector<T> average_ensemble2D(const T* data_ptr, int width, int height, int channels, int x, int y, int ensemble_size);


#endif