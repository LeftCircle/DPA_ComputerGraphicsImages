#ifndef LUT_H
#define LUT_H


#include <vector>

template <typename T>
class LUT {
public:
    LUT(T&&... values) : _table{std::forward<T>(values)...} {}

    // Given a value from 0 -> 1, returns the lerped value of the two 
    // surrounding LUT items that this is between. 
    // IE, if the LUT only has two items, then a value of 0.5 will return
    // the lerp between the two values. 
    T lerp(float val);

private:
    std::vector<T> _table;
};

#endif