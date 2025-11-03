#include "vector.hpp"
#include <iostream>

int main () {
	vector<int> v = {67, 2, 8, 4, 5};

	for (int i = 0; i < v.size(); i++) {
		std::cout << v[i] << std::endl;
	}
}