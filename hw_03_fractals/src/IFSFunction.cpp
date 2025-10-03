#include "IFSFunction.h"



Point Spherical::operator()(const Point& P) const {
	float rsq = P.x * P.x + P.y * P.y;
	if (abs(rsq) < EPSILON){
		return P;
	}
	return Point(P.x / rsq, P.y / rsq);
}







void apply_random_func(int iters, std::vector<IFSFunction*>& function_ptrs, std::vector<Color>& colors,
						Point& point, Color& color){
	int n_colors = colors.size();
	int n_funcs = function_ptrs.size();
	for (int i = 1; i <= iters; i++){
		IFSFunction* rand_func = function_ptrs[lrand48() % n_funcs];
		Color rand_color = colors[lrand48() % n_colors];
		color = (color + rand_color) / 2.0f;
		

	}
}
