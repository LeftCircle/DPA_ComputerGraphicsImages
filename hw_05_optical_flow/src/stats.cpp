#include "stats.h"



template<typename T>
std::vector<T> average_ensemble2D(const T* data_ptr, int width, int height, int channels, int x, int y, int ensemble_size) {
    std::vector<T> avg_vals(channels, 0);
    float total = static_cast<float>(2 * ensemble_size + 1) * static_cast<float>(2 * ensemble_size + 1);
    for (int j = -ensemble_size; j <= ensemble_size; ++j) {
        for (int i = -ensemble_size; i <= ensemble_size; ++i) {
            int xi = x + i;
            int yj = y + j;
            if (xi < 0 || xi >= width || yj < 0 || yj >= height) {
                continue;
            }
            long index = (yj * width + xi) * channels;
            for (int c = 0; c < channels; ++c) {
                avg_vals[c] += data_ptr[index + c];
            }
        }
    }

    for (int c = 0; c < channels; ++c) {
        avg_vals[c] /= total;
    }
    return avg_vals;
}



// Template function definition
template std::vector<float> average_ensemble2D<float>(const float* data_ptr, int width, int height, int channels, int x, int y, int ensemble_size);
 